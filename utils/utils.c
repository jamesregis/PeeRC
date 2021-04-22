void add_peer(struct peer ** p, char * htoname, u_short portnum) {
    struct peer * p2, *oldp;
    oldp = *p;
    p2 = malloc(sizeof(struct peer));
#ifdef DEBUG
    printf("Dans add_peer... \n"); 
#endif
    if (!p2)
        exit(EXIT_FAILURE);
    p2->portnum = portnum;
    printf("htoname -> %s, sur le port %d\n", htoname, portnum);
    p2->htoname = htoname;
    p2->prev = oldp;
    *p = p2;

}

void print_peer_list(struct peer * p) {
    int i;
    printf("Dans print_peer_list ... \n"); 
    i = 0;
    while (p != NULL) {
        printf("liste_peer %d: %d, %s, %d\n", p, i, p->htoname, p->portnum);
        i +=1;
        p = p->prev;
    }

}

/* find a peer by its login and return the socket number to communicate */
struct peer * get_peer_info(struct peer * p, char * login) {
    while (p != NULL) {
        if (strcmp(login, (p)->login) == 0)
            return(p);
        else
            p = (p)->prev;
    }
    return NULL;
}


/* find a peer and remove all information about this peer */
void remove_peer(struct peer ** p, char * login) {
    while (*p != NULL) {
        if (strcmp(login, (*p)->login) == 0)
            (*p) = (*p)->prev;
    }
}



/* to test if a client is still alive */
int check_peer() {
    return(0);
}


/* to test if a peer is already register : a same login name is find */
int check_registered_peer() {
    return(0);
}

int parse_message(struct server * srv, char * buff, struct peer **p) {
    
    /* printf("%c\n", buff[0]);*/
    u_long fromaddr;

    /* A client want to login */
    if (strncmp(buff + HEADER_SIZE, LOGIN_MESSAGE, HEADER_SIZE) == 0) {
        printf("login_message\n");
        print_peer_list(*p);
        fromaddr = srv->from->sin_addr.s_addr;
        printf("From %s port %d\n", (gethostbyaddr((char *)&fromaddr, sizeof(srv->fromaddr), AF_INET))->h_name, srv->from->sin_port);
        add_peer(p, (gethostbyaddr((char *)&fromaddr, sizeof(srv->fromaddr), AF_INET))->h_name, srv->from->sin_port);
#ifdef DEBUG
        print_peer_list(*p);
#endif
    }
    /* you receive a logout message from a peer */
    if (strncmp(buff, LOGOUT_MESSAGE, 1) == 0)
        printf("logout_message\n");

    /* you receive a public message from a peer */
    if (strncmp(buff, PUBLIC_MESSAGE, 1) == 0)
        printf("public_message\n");

    /* you receive a private message from a peer */
    if (strncmp(buff, PRIVATE_MESSAGE, 1) == 0)
        printf("private_message\n");

    /* you receive receive a list request from a peer */
    if (strncmp(buff, R_PEER_LIST, 1) == 0)
        printf("r_peer_lits_message\n");

    /* you receive a list request answer from a peer */
    if (strncmp(buff, A_PEER_LIST, 1) == 0)
        printf("a_peer_list_message\n");

    return(0);

}
