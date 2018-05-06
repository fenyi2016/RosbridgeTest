#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/time.h>

#include <json/json.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static size_t strcat2(char **dst_out, ...)
{
    size_t len = 0, len_sub;
    va_list argp;
    char *src;
    char *dst = NULL, *dst_p;

    *dst_out = NULL;

    va_start(argp, dst_out);
    for (;;)
    {
        if ((src = va_arg(argp, char *)) == NULL) break;
        len += strlen(src);
    }
    va_end(argp);

    if (len == 0) return 0;

    dst = (char *)malloc(sizeof(char) * (len + 1));
    if (dst == NULL) return -1;
    dst_p = dst;

    va_start(argp, dst_out);
    for (;;)
    {
        if ((src = va_arg(argp, char *)) == NULL) break;
        len_sub = strlen(src);
        memcpy(dst_p, src, len_sub);
        dst_p += len_sub;
    }
    va_end(argp);
    *dst_p = '\0';

    *dst_out = dst;

    return len;
}


int main(){
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_port = htons(9090);  

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char* advmsg = "{"
        "\"op\":\"advertise\","
        "\"topic\":\"/talker3\","
        "\"type\":\"geometry_msgs/PoseStamped\""
    "}";



    // char* pose = "{"
    //     "\"position\" : { \"x\":1, \"y\":2, \"z\":3 },"
    //     "\"orientation\" : { \"x\":1, \"y\":2, \"z\":3, \"w\":4 }"
    // "}";

    // char* data = "{"
    //     "\"pose\" : pose"
    // "}";


    // char* pubmsg = "{"
    //     "\"op\":\"publish\","
    //     "\"topic\":\"/talker3\","
    //     "\"msg\":data"
    // "}";

    // [Todo] Add timestamp to header
    // struct timeval tp;
    // gettimeofday(&tp, NULL);
    // std::cout << tp.tv_sec << "," << tp.tv_usec << std::endl;

    double px = 1.0;
    double py = 2.0;
    double pz = 3.0;
    double ox = 1.0;
    double oy = 2.0;
    double oz = 3.0;
    double ow = 4.0;

    char *pubmsg = NULL;
    char *pose = NULL;
    char *data = NULL;

    size_t len;

    char  pxc[64];
    char  pyc[64];
    char  pzc[64];
    char  oxc[64];
    char  oyc[64];
    char  ozc[64];
    char  owc[64];

    sprintf(pxc, "%g", px);
    sprintf(pyc, "%g", py);
    sprintf(pzc, "%g", pz);
    sprintf(oxc, "%g", ox);
    sprintf(oyc, "%g", oy);
    sprintf(ozc, "%g", oz);
    sprintf(owc, "%g", ow);

    len = strcat2(&pose, " { \"position\" : { \"x\" : ", pxc , ", \"y\" : ", pyc, ", \"z\" : ", pzc, " } ,", 
                         "   \"orientation\" : { \"x\" : ", oxc , " , \"y\" : ", oyc, ", \"z\" : ", ozc, " , \"w\" :", owc, "} }", NULL);
    len = strcat2(&data, " { \"pose\" : ",  pose , "}", NULL);
    len = strcat2(&pubmsg, "{ \"op\" : \"publish\" , \"topic\" : \"/talker3\" , \"msg\" : ", data, "}", NULL);


    Json::Reader  reader;
    Json::Value   advjson;
    Json::Value   pubjson;
    Json::StyledWriter writer;

    reader.parse(advmsg, advjson);
    std::string advBuf = writer.write(advjson);

    reader.parse(pubmsg, pubjson);
    std::string pubBuf = writer.write(pubjson);

    // Debug
    // std::cout << advmsg << std::endl;
    // std::cout << pubmsg << std::endl;

    // std::cout << advjson << std::endl;
    // std::cout << pubjson << std::endl;

    // std::cout << advBuf.c_str() << std::endl;
    // std::cout << pubBuf.c_str() << std::endl;



    send(sock, advBuf.c_str(), advBuf.size(), 0);

    for(int i = 0; i<3; i++){
        send(sock, pubBuf.c_str(), pubBuf.size(), 0);
    }
   
   
    
    close(sock);

    if (data != NULL) free(data);
    if (pose != NULL) free(pose);
    if (pubmsg != NULL) free(pubmsg);

    return 0;
}

