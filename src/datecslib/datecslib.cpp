    //
    // Created by danil on 14.02.23.
    //

#include <sys/poll.h>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../include/datecsException/datecsException.h"
#include "../../include/datecslib/datecslib.h"
#include "../../include/helpFunc/helpFunc.h"

    bool datecslib::openPort(string port) {
        closePort();
        seq = 1;
        fd = open(port.c_str() , O_RDWR | O_NOCTTY);
        init_tty(fd);
        return fd==-1? false: true;
    }

    datecslib::datecslib(string port) {
        openPort(port);
    }

    bool datecslib::isOpen() {
        return fd==-1? false: true;
    }

    bool datecslib::sendCommand(int command, string data) {
        answerFromPrinter = "Status error message";
        if(!isOpen()){
            return false;
        }
        int len=25;
        unsigned char *arr = CommandFormation(command, data ,&len);
        int nw = write(fd, arr, len);
        if ( nw < 0)
        {
            return false;
        }
        return true;
    }

    void datecslib::init_tty(int DriverHandle) {

            struct termios options;
            struct termios termios_save;
            // Get the current options for the port...
            tcgetattr(DriverHandle, &options);
            memcpy(&termios_save, &options, sizeof(struct termios));
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);

            // Enable the receiver and set local mode...
            options.c_cflag     &=  ~PARENB;            // Make 8n1
            options.c_cflag     &=  ~CSTOPB;
            options.c_cflag     &=  ~CSIZE;
            options.c_cflag     |=  CS8;

            options.c_cflag     &=  ~CRTSCTS;           // no flow control
            options.c_cc[VMIN]   =  1;                  // read doesn't block
            options.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
            options.c_cflag     |=  CREAD | CLOCAL;
            cfmakeraw(&options);
            // Set the new options for the port...
            tcsetattr(DriverHandle, TCSANOW, &options);
            fcntl(fd, F_SETFL, FNDELAY);
            tcflush(DriverHandle, TCOFLUSH);
            tcflush(DriverHandle, TCIFLUSH);
    }

    u_char *datecslib::CommandFormation(ushort command, string data, int *lenght) {
        if(seq==1)
            seq = ushort(GetRandomNumber(32,127));
        if(seq>=127)
            seq=u_char(32);
        //Массив для передачи на порт
        std::vector<ushort> vec;
        ////std::copy(input.begin(), input.end(), arr);
        //<01>
        vec.push_back(ushort(0x1));
        int len;
        ushort *command_ushort = string_to_ushort(data, &len);
        ushort num = ushort(36 + len);
        //<len>
        vec.push_back(num);
        //<seq>
        seq += u_char(1);
        vec.push_back(seq);
        //Добавить команду <cmd>
        vec.push_back(ushort(command));
        //Указатель на массив байтов из строки сmass = {std::ushort *} 0x55555556b6d0  командой

        for (int i = 0; i < len; ++i) {
            //Добавление в общий массив <data>
            vec.push_back(command_ushort[i]);
        }
        delete[] command_ushort;
        //<05>
        vec.push_back(ushort(5));
        //<bcc>
        command_ushort = CsCount(vec);
        for (int i = 0; i < 4; ++i) {
            //Добавление в общий массив <bcc>
            vec.push_back(command_ushort[i]);
        }
        delete[] command_ushort;
        //<03>
        vec.push_back(ushort(3));
        *lenght = vec.size();
        u_char  *arr = new u_char[*lenght];
        copy(vec.begin(), vec.end(), arr);
        return arr;
    }

    ushort *datecslib::CsCount(const vector<ushort> &array) {
        int len = array.size();
        ushort *numArray = new ushort[4];
        ushort num1 = 0;
        for (int i = 0; i < len; i++) {
            if(array[i] == ushort(1))
                continue;
            if (array[i] == ushort(5)) {
                num1 += ushort(array[i]);
                break;
            }
            num1 += ushort(array[i]);
        }
        for (int i = 0; i <= 3; ++i) {
            int num3 = (int(num1 & 15)) + 48;
            num1 >>= 4;
            numArray[3 - i] = ushort(num3);
        }
        return numArray;
    }

    datecslib::datecslib() : fd(-1){

    }

    void datecslib::closePort() {
        if(isOpen()) {
            close(fd);
            fd = -1;
        }
    }

    datecslib::~datecslib() {
        close(fd);
    }

    bool datecslib::readAnswer() {
        u_char c;
        if (!readFirstByte(c))
            return false;

        int ret = 0;
        status.clear();
        char buff;
        //buff[0] = c;
        vector<char> vec;
        struct pollfd fds;
        fds.fd = fd;
        fds.events = POLLIN;
        poll(&fds, 1, 50);
        while (fds.revents & POLLIN) {
                ret = read(fd, &buff, 1);
                vec.push_back(buff);
                if (ret < 0) {
                    return false;
                }
                poll(&fds, 1, 50);
        }
        string bittest;

        int count = 0;
        for (int i = vec[0] - 7 - 0x20; i < vec[0] - 0x20; i++) {
            bittest = std::bitset<8>(vec[i]).to_string();
            reverse(bittest.begin(), bittest.end());
            transcriptStatus(bittest, count);
            count++;
        }
        for (int i = 3; i <= int(vec[0]) - 8 - 0x20; i++) {
            if(i==3) {
                answerFromPrinter.clear();
            }
            answerFromPrinter += vec[i];
        }
        return true;

    }

    void datecslib::transcriptStatus(string statusByte, int num) {
        switch (num) {
            case 0: {
                if (statusByte[0] == '1')
                    status.push_back("Синтаксическая ошибка.");
                if (statusByte[1] == '1')
                    status.push_back("Недопустимая команда.");
                if (statusByte[2] == '1')
                    status.push_back("Не установлены дата/время.");
                if (statusByte[3] == '1')
                    status.push_back("Дисплей покупателя не подключен.");
                if (statusByte[4] == '1')
                    status.push_back("SAM не от этого устройства (регистратор не персонализирован).");
                if (statusByte[5] == '1')
                    status.push_back("Общая ошибка или все ошибки, обозначенные \"#\".");
                break;
            }
            case 1: {
                if (statusByte[0] == '1')
                    status.push_back("При выполнении команды произошло переполнение поля суммы.");
                if (statusByte[1] == '1')
                    status.push_back("Выполнение команды не допускается в текущем фискальном режиме");
                if (statusByte[2] == '1')
                    status.push_back("Аварийное обнуление RAM.");
                if (statusByte[3] == '1')
                    status.push_back("Открыт возвратный чек.");
                if (statusByte[4] == '1')
                    status.push_back("Ошибка SAM.");
                if (statusByte[5] == '1')
                    status.push_back("Крышка принтера открыта.");
                break;
            }
            case 2: {
                if (statusByte[0] == '1')
                    status.push_back(" Бумага закончилась.");
                if (statusByte[1] == '1')
                    status.push_back("Заканчивается бумага.");
                if (statusByte[2] == '1')
                    status.push_back("Носитель КЛЭФ заполнен (Осталось менее 1 МБ).");
                if (statusByte[3] == '1')
                    status.push_back("Открыт фискальный чек.");
                if (statusByte[4] == '1')
                    status.push_back("Носитель КЛЭФ приближается к заполнению (осталось не более 2 МБ).");
                if (statusByte[5] == '1')
                    status.push_back("Открыт нефискальный чек.");
                if(statusByte[6] == '1')
                    status.push_back("Носитель КЛЭФ почти заполнен (осталось примерно 1 МБ, возможна печать только отдельных чеков).");
                break;
            }
            case 3: {
                break;
            }
            case 4: {
                if (statusByte[0] == '1')
                    status.push_back("В фискальной памяти присутствуют ошибки.");
                if (statusByte[1] == '1')
                    status.push_back("Нетрудоспособное фискальная память.");
                if (statusByte[3] == '1')
                    status.push_back("Осталось менее 50 записей в фискальную память.");
                if (statusByte[4] == '1')
                    status.push_back("Заполнение фискальной памяти.");
                break;
            }
        }

    }

    vector<string> datecslib::getStatusByte() {
        return status;
    }

    bool datecslib::readFile(string path) {
        struct tm *u;
        const time_t timer = time(NULL);
        u = localtime(&timer);

        int cmd;
        string data;
        fileRead.open(path);
        if(!fileRead.is_open()) {
            return false;
        }
        while(!fileRead.eof()){
            fileRead >> cmd;
            getline(fileRead,data);
            data.erase(0,1);
            string cp = data;
            cp.erase(remove(cp.begin(),cp.end(),' '),cp.end());
            if (cmd == 61 && to_lower(data)=="setdatetimefrompc")
                data = date_to_printer_format(u);
            if(cmd == 115)
                data = cp;
            if(cp=="null")
                data = "";
            commandFromFile.push_back({cmd,data});
        }
        fileRead.close();
        return true;

    }

    list<cmdData> datecslib::getCommandFromFile() {
        return commandFromFile;
    }

    bool datecslib::sendRead(int command, string data) {
        if(!sendCommand(command,data)) {
            return false;
        }
        if(!readAnswer()) {
            return false;
        }
        return true;
    }

    string datecslib::getanswerFromPrinter() {
        return answerFromPrinter;
    }

    bool datecslib::readFirstByte(u_char &c) {

        int ret;
        struct pollfd fds;
        c = 22;
        fds.fd=fd;
        int count = 0;
        fds.events = POLLIN;
        while(c == 22){
        poll(&fds, 1, 500);
        if (fds.revents & POLLIN) {
            ret = read(fd, &c, 1);
            count++;
        }
        else{
            throw datecsException("Timeout Exception.\nПК ожидает ответа от фискального регистратора в течение 500 ms, после чего определяет состояние таймаута.", "timeout");
        }
        if(c == 21)
            throw datecsException("NAK Exception.\nВозникла ошибка формата сообщения или контрольная сумма неверна.");
        if(count>5000){
            throw datecsException("SYN Exception.\nВ случае, если регистратор отправляет символ 16H дольше 5 секунд - необходимо повторить сообщение со следующим порядковым номером.");
        }
        }
        return true;
    }

    bool datecslib::connetSocket(string ip) {
        struct sockaddr_in serv_addr;

        closePort();

        if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            throw datecsException("Could not create socket");
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(9100);

        if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
        {
            throw datecsException("inet_pton error occured");
        }

        if( connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            throw datecsException("Connect Failed in open");
        }
        return true;
    }

    bool datecslib::sendCommandSock(int command, string data) {
        int len=25;
        unsigned char *arr = CommandFormation(command, data ,&len);
        int nw = send(fd, arr, len, 0);
        if ( nw < 0)
        {
            return false;
        }
        return true;
    }


