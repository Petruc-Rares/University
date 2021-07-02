#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[]) {
    char *message;
    char *response;
    int sockfd, sockfd_weather;

    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);

    // Ex 1.1: GET dummy from main server
    message = compute_get_request("34.118.48.238", "/api/v1/dummy", NULL, NULL, 0);
    // puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // puts(response);

    // Ex 1.2: POST dummy and print response from main server
    char **form_data = calloc(2, sizeof(char *));
    for (int i = 0; i < 2; i++) {
        form_data[i] = calloc(LINELEN, sizeof(char));
    }
    strcpy(form_data[0], "username=student");
    strcpy(form_data[1], "password=student");
    message = compute_post_request("34.118.48.238", "/api/v1/dummy",
                                   "application/x-www-form-urlencoded", form_data, 2, NULL, 0);
    // puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // puts(response);

    // Ex 2: Login into main server
    message = compute_post_request("34.118.48.238", "/api/v1/auth/login",
                                   "application/x-www-form-urlencoded", form_data, 2, NULL, 0);
    puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Parse cookie
    char *cookie_temp = strstr(response, "Set-Cookie: connect.sid=");
    char *cookie = malloc(LINELEN);
    sscanf(cookie_temp, "Set-Cookie: %s", cookie);
    cookie[strlen(cookie) - 1] = '\0';
    puts(cookie);

    // Ex 3: GET weather key from main server
    message = compute_get_request("34.118.48.238", "/api/v1/weather/key", NULL, &cookie, 1);
    puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 4: GET weather data from OpenWeather API - BONUS
    sockfd_weather = open_connection("188.166.16.132", 80, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request("188.166.16.132", "/data/2.5/weather",
                                  "lat=44.4397&lon=26.0963&appid=b912dd495585fbf756dc6d8f415a7649",
                                  &cookie, 1);
    puts(message);

    send_to_server(sockfd_weather, message);
    response = receive_from_server(sockfd_weather);
    puts(response);

    // Ex 5: POST weather data for verification to main server - BONUS
    char *json_response = basic_extract_json_response(response);
    message = compute_post_request("34.118.48.238", "/api/v1/weather/44.4397/26.0963",
                                   "application/json", &json_response, 1, &cookie, 1);
    puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // BONUS: make the main server return "Already logged in!"
 /*   message = compute_post_request("34.118.48.238", "/api/v1/auth/login",
                                   "application/x-www-form-urlencoded", form_data, 2, &cookie, 1);
    puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 6: Logout from main server
    message = compute_get_request("34.118.48.238", "/api/v1/auth/logout", NULL, &cookie, 1);
    puts(message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
*/
    // free the allocated data at the end!
    free(cookie);
    for (int i = 0; i < 2; i++) {
        free(form_data[i]);
    }
    free(form_data);
    return 0;
}
