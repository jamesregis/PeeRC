int init_host_socket(struct peer * client) {
    client->to = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    
/*    if((client->toinfo = getpeerbyname(client->htoname)) == NULL) {
        fprintf(stderr,"Error %d in getpeerbyname: %s\n", errno, strerror(errno));
        exit(errno);
    };
*/
    client->toinfo = gethostbyname(client->htoname);

    client->addr_in_size = sizeof(struct sockaddr_in);
    memset((char *)client->to, (char)0, client->addr_in_size);
  
  
    client->toaddr = *(u_long *)client->toinfo->h_addr_list[0];

    client->to->sin_family = AF_INET;
    client->to->sin_addr.s_addr = client->toaddr;
    client->to->sin_port = /*(htons)*/client->portnum;

    if((client->sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        fprintf(stderr,"Error %d in socket: %s\n", errno, strerror(errno));
        exit(errno);
    };
    return(0);
}


int init_server_socket(struct server * srv) {
    int addr_in_size;

    addr_in_size = sizeof(struct sockaddr_in);
    srv->from = (struct sockaddr_in *)malloc(addr_in_size); 
    srv->my_addr = (struct sockaddr_in *)malloc(addr_in_size);

    memset((char *)srv->my_addr,(char)0,addr_in_size);
    srv->my_addr->sin_family = AF_INET;
    srv->my_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    srv->my_addr->sin_port = htons(srv->portnum);

    if((srv->sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        fprintf(stderr,"Error %d in socket: %s\n", errno, strerror(errno));
        exit(errno);
    };

    if(bind(srv->sockfd, (struct sockaddr *)srv->my_addr, addr_in_size) < 0){
        fprintf(stderr,"Error %d in bind: %s\n", errno, strerror(errno));
        if(errno != EADDRINUSE)
            exit(errno);
    };

    fprintf(stdout,"Ready to receive on port %d\n", srv->portnum);
    return (0);
}

/* send a message to all peer */
int send_to_group(struct peer *client, char * msg, char * action_type) {
    int i;   
    char msg2[MSG_SIZE];
    
    /*if(sendto(client->sockfd, msg, strlen(msg), 0, \
     * (struct sockaddr *)client->to, client->addr_in_size) == -1){
            fprintf(stderr,"Error %d in sendto: %s\n", errno, strerror(errno));
            exit(errno);
    */
    strcat(msg2, action_type);
    strcat(msg2, DEFAULT_LOGIN);
    strcat(msg2, "PUBLIC");
    strcat(msg2, msg);

    for(i = 0; i < 3/*A revoir*/ ; i++ ){
        if(sendto(client[i].sockfd, msg, strlen(msg), 0, \ 
                    (struct sockaddr *)client[i].to, \ 
                    client[i].addr_in_size) == -1){
            fprintf(stderr,"Error %d in global sendto: %s\n", errno, \ 
                    strerror(errno));
            exit(errno);
        }
    }
    return(0);
}

/* to send a message to one peer */
int send_to_peer(struct peer * p, char * login, char * msg, char * action_type) {
    struct peer * p2;
    char msg2[MSG_SIZE];

    p2 = NULL;
    
    p2 = get_peer_info(p, login);
    
    if(p2 != NULL) {
        strcat(msg2, action_type);
        strcat(msg2, DEFAULT_LOGIN);
        strcat(msg2, "PRIVATE");
        strcat(msg2, msg);
        
        if (sendto(p2->sockfd, msg2, strlen(msg2), 0, \ 
                    (struct sockaddr *)p2->to, p2->addr_in_size) == -1) {
            fprintf(stderr,"Error %d in individual sendto: %s\n", errno, \ 
                    strerror(errno));
            exit(errno);
        }
    }
    return(0);
}
