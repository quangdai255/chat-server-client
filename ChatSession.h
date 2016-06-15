/*
 * ChatSession.h
 *
 *  Created on: Jun 15, 2016
 *      Author: nduytg
 */

#ifndef CHATSESSION_H_
#define CHATSESSION_H_

#include <arpa/inet.h>
#include <netinet/in.h>

//Class quan ly 1 phien chat giua 2 client
class ChatSession
{
	private:
		class Peer
		{
			public:
				char IP[INET6_ADDRSTRLEN];
				int port;
		};

	public:
		ChatSession();
		virtual ~ChatSession();

		Peer peer1;
		Peer peer2;

};

#endif /* CHATSESSION_H_ */
