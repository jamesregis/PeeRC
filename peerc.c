#include "peerc.h"
#include "utils/utils.c"
#include "utils/network.c"


int main(int argc, char **argv) {

    fd_set readfds;
    int maxfd, retval;
    socklen_t addr_size;
    char buff[512], message[512];
    char msg[] = "this-is-a-test-message";

    /* create server */
    struct server * srv;

    /* create client list */
    struct peer * peer_list;
    /* peer_list = NULL;*/


    maxfd = 0;
    addr_size = sizeof(struct sockaddr_in);
    
    FD_ZERO(&readfds);
        
    printf("\nPeer-to-Peer Relay Chat\n");
    peer_list = malloc(sizeof(struct peer));
    srv = malloc(sizeof(struct server));

    srv->portnum = htons(DEFAULT_PORT_NUMBER);
    init_server_socket(srv);
    /* local server is ready */

    /* first connexion initialization */
    peer_list->portnum  = htons(12345);
    peer_list->htoname = DEFAULT_SERVER;
    /* peer_list->login = ; */
    peer_list->prev = NULL;
    
    init_host_socket(peer_list);
    /* printf("peer_list->portnum : %d, %s, %d\n", peer_list->portnum, \
     * peer_list->toinfo, peer_list->sockfd);*/
    sendto(peer_list->sockfd, msg, sizeof(msg), 0,\ 
            (struct sockaddr *)peer_list->to, peer_list->addr_in_size); 


    while(1)
    {
        FD_SET(srv->sockfd, &readfds);
        FD_SET(fileno(stdin), &readfds);

        maxfd = max(fileno(stdin), srv->sockfd);
        retval = select(maxfd+1, &readfds, NULL, NULL, NULL);

        if (retval == -1)
            perror("select()");

        if FD_ISSET(srv->sockfd, &readfds) {
            recvfrom(srv->sockfd, buff, MSG_SIZE, 0, \ 
                    (struct sockaddr *)srv->from, &addr_size);
            parse_message(srv, buff, &peer_list); 
            /* lecture et decodage du message */
            printf("client> %s\n", buff);
        }
        else if FD_ISSET(0, &readfds) {
            fgets(message, sizeof(message), stdin);
            /* fonction d'envoie de message */
            /* sendto(peer_list->sockfd, message, sizeof(message), 0,\
             * (struct sockaddr *)peer_list->to, peer_list->addr_in_size); */
            send_to_group(peer_list, message, LOGIN_MESSAGE);
            printf("you> %s\n", message);

        }
    }
    return(0);
}
