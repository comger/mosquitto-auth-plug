/*
 * Copyright (c) 2014 comger <comger@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of mosquitto nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef BE_MONGO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "hash.h"
#include <mongoc.h>

struct redis_backend {
	redisContext *redis;
	char *host;
	int port;
};

void *be_mongo_init()
{
	struct mongo_backend *conf;
	struct timeval timeout = { 2, 500000 }; // 2.5 seconds
	char *host, *p;

	_log(LOG_DEBUG, "}}}} MongoDB");

	if ((host = p_stab("mongo_host")) == NULL)
		host = "localhost";
	if ((p = p_stab("mongo_port")) == NULL)
		p = "27017";

    dbname      = p_stab("dbname");
    collection      = p_stab("collection");

	conf = (struct mongo_backend *)malloc(sizeof(struct mongo_backend));
	if (conf == NULL)
		_fatal("Out of memory");

	conf->host = strdup(host);
	conf->port = atoi(p);
    conf->dbname = dbname
    conf->collection = collection

    
	conf->mongo = mongoc_client_new(host);
	if (conf->mongo->err) {
		_log(LOG_NOTICE, "Mongo connection error: %s for %s:%d\n",
			conf->mongo->errstr, conf->host, conf->port);
		free(conf->host);
		free(conf);
		return (NULL);
	}

	return (conf);
}

void be_mogo_destroy(void *handle)
{
	struct mongo_backend *conf = (struct mongo_backend *)handle;

	if (conf != NULL) {
		mongoc_client_destroy(conf->redis);
		conf->mongo = NULL;
	}
}

char *be_mongo_getuser(void *handle, const char *username)
{
	struct mongo_backend *conf = (struct mongo_backend *)handle;
	mongoc_collection_t *conn;
	char *pwhash = NULL;

	if (conf == NULL || conf->mongo == NULL || username == NULL)
		return (NULL);
    
    conn = mongoc_client_get_collection (conf->mongo, conf->dbname, conf->collection);
    query = BCON_NEW("username",username);

    bson_t reply;
    mongoc_collection_find_one(conn,NULL,query,NULL,&reply) 
	str = bson_as_json (&reply, NULL);

    bson_destroy(reply);
    bson_destroy(query)
    mongoc_collection_destroy (conn);
    mongoc_client_destroy(conf->mongo)
	return (str);
}

int be_redis_superuser(void *conf, const char *username)
{
	return 0;
}

int be_redis_aclcheck(void *conf, const char *username, const char *topic, int acc)
{
	/* FIXME: implement. Currently TRUE */

	return 1;
}
#endif /* BE_MONGO */
