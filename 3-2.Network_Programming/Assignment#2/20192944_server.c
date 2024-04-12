#include "20192944_server.h"

void errProc(const char *str);
void errPrint(const char *str);

int main(int argc, char **argv)
{
    int srvSd, clntSd;
    struct sockaddr_in srvAddr, clntAddr;
    int clntAddrLen, readLen, strLen;

    pid_t pid;

    if (argc != 2)
    {
        printf("[*] Usage: %s [port] \n", argv[0]);
        exit(1);
    }
    printf("[*] Server start...\n");

    srvSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srvSd == -1)
        errProc("[*] ERROR");

    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(atoi(argv[1]));

    if (bind(srvSd, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
        errProc("[*] ERROR");
    if (listen(srvSd, 5) < 0)
        errProc("[*] ERROR");
    clntAddrLen = sizeof(clntAddr);
    while (1)
    {
        clntSd = accept(srvSd, (struct sockaddr *)&clntAddr, &clntAddrLen);
        if (clntSd == -1)
        {
            errProc("[*] ERROR");
            continue;
        }
        printf("[*] Client %s:%d is connected...\n",
               inet_ntoa(clntAddr.sin_addr),
               ntohs(clntAddr.sin_port));
        pid = fork();
        if (pid == 0)
        { // Child process for each client

            close(srvSd);
            send(clntSd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
            while (1)
            {
                send(clntSd, MENU, strlen(MENU), 0);
                char rBuff[BUFSIZ];
                char wBuff[BUFSIZ];
                readLen = recv(clntSd, rBuff, sizeof(rBuff) - 1, 0);
                if (readLen == 0)
                    break;
                rBuff[readLen] = '\0';
                printf("[*] Client(%d): %s\n",
                       ntohs(clntAddr.sin_port), rBuff);

                if (strcmp(rBuff, "1") == 0)
                {
                    time_t timer;
                    time(&timer);
                    send(clntSd, ctime(&timer), strlen(ctime(&timer)), 0);
                }
                else if (strcmp(rBuff, "2") == 0)
                {
                    float32_t a, b, result;
                    char operator;

                    send(clntSd, CALCULATOR_MENU, strlen(CALCULATOR_MENU), 0);

                    readLen = recv(clntSd, rBuff, sizeof(rBuff) - 1, 0);
                    if (readLen == 0)
                        break;
                    rBuff[readLen] = '\0';
                    printf("[*] Client(%d): %s\n",
                           ntohs(clntAddr.sin_port), rBuff);

                    sscanf(rBuff, "%f %c %f", &a, &operator, & b);
                    switch (operator)
                    {
                    case '+':
                        result = a + b;
                        break;
                    case '-':
                        result = a - b;
                        break;
                    case '*':
                        result = a * b;
                        break;
                    case '/':
                        if (a == 0 || b == 0)
                        {
                            strcpy(rBuff, "[Error: Division by zero]");
                            if (write(clntSd, rBuff, strlen(rBuff)) < 0)
                            {
                                perror("[Error writing to socket]");
                            }
                            continue;
                        }
                        else
                        {
                            result = a / b;
                        }
                        break;
                    default:
                        strcpy(wBuff, "[Error: Invalid operator]");
                        if (write(clntSd, wBuff, strlen(wBuff)))
                        {
                            perror("[Error writing to socket]");
                        }
                        continue;
                    }
                    sprintf(wBuff, "[ %.2f %c %.2f = %.2f ]", a, operator, b, result);
                    printf("%s\n", wBuff);
                    send(clntSd, wBuff, strlen(wBuff), 0);
                }
                else if (strcmp(rBuff, "q") == 0)
                {
                    memcpy(wBuff, rBuff, strlen(rBuff));
                    send(clntSd, wBuff, strlen(rBuff), 0);
                    break;
                }
                else
                {
                    memcpy(wBuff, rBuff, strlen(rBuff));
                    send(clntSd, wBuff, strlen(wBuff), 0);
                }
            }
            printf("[*] Client(%d): disconnected\n",
                   ntohs(clntAddr.sin_port));
            close(clntSd);
            return 0;
        }
        else if (pid == -1)
            errProc("fork");
        else
        { // Parent process
            wait(NULL);
            close(clntSd);
        }
    }
    close(srvSd);
    return 0;
}

void errProc(const char *str)
{
    fprintf(stderr, "%s: %s \n", str, strerror(errno));
    exit(1);
}

void errPrint(const char *str)
{
    fprintf(stderr, "%s: %s \n", str, strerror(errno));
}

void sigintHandler(int signum)
{
    printf("\n[*] Received SIGINT. Exiting gracefully...\n");
    exit(0);
}