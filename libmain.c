#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <nss.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "instances.h"

static enum nss_status not_enough(int *errnop, int *h_errnop)
{
    *errnop = ERANGE;
    *h_errnop = NETDB_INTERNAL;
    return NSS_STATUS_TRYAGAIN;
}

enum nss_status _nss_ec2_gethostbyname_r(const char *name, struct hostent *result_buf, char *buf, size_t _buflen, int *errnop, int *h_errnop)
{
    char *p = buf;
    size_t len;
    uint32_t ip;
    ssize_t buflen = (ssize_t)_buflen;

    if (buflen == 0) return not_enough(errnop, h_errnop);

    ip = htonl(FindAddress(name));
    if (ip == 0u) {
        *errnop = ENOENT;
        return NSS_STATUS_NOTFOUND;
    }

    // copy name
    len = snprintf(p, buflen, "%s", name);
    result_buf->h_name = p;
    p += len;
    buflen -= len;

    // alias (no set)
    if (buflen < (ssize_t)sizeof(char *)) return not_enough(errnop, h_errnop);

    ((char **)p)[0] = NULL;
    result_buf->h_aliases = (char **)p;
    p += sizeof(char *);
    buflen -= sizeof(char *);

    if (buflen == 0) return not_enough(errnop, h_errnop);

    // address type (2, AF_INET)
    result_buf->h_addrtype = AF_INET;

    // length (4, 32bit=IPv4 Address)
    result_buf->h_length = 4;

    // address
    if (buflen < (ssize_t)sizeof(char *) * 2) return not_enough(errnop, h_errnop);

    result_buf->h_addr_list = (char **)p;
    p += sizeof(char *) * 2;
    buflen -= sizeof(char *) * 2;

    if (buflen < 4) return not_enough(errnop, h_errnop);

    result_buf->h_addr_list[0] = p;
    memcpy(p, &ip, 4);
    p += 4;
    buflen -= 4;

    //if (buflen == 0) return not_enough(h_errnop);

    result_buf->h_addr_list[1] = NULL;

    return NSS_STATUS_SUCCESS;
}


enum nss_status
    _nss_ec2_gethostbyname2_r(const char *name, int af, struct hostent *result_buf,
    char *buf, size_t buflen, int *errnop, int *h_errnop)
{
    if (af != AF_INET) return NSS_STATUS_NOTFOUND;
    return _nss_ec2_gethostbyname_r(name, result_buf, buf, buflen, errnop, h_errnop);
}
