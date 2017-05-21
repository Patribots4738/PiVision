#pragma once
#include "stdafx.h"
#include <string>

class UDPManager {
	public:
		UDPManager(std::string ip); // Each manager is in charge of handling 1 IP to send to a different one make a different Manager
									// Is that the best way? NO, but it suits our purpose because we only want to send it to one device

		int send(std::string data); // Does what you might think Sends the data that you put into it to the IP the manager is in charge with.
									// Format of message should be (without spaces):
									// ActualHeight, PixelHeight, andgle, distance, centerx, centerY,

		std::string listen();		// Promply listens on a seperate thread for any incoming messages.
									// If this gets called twices I guess both will return the same message, so I guess it would be better if a
									// callback system was in place. 
									//
									// LOL who am I kidding, we both know that this code is never gonna get ran and if it does it'll do nothing
									// cause I'm gonna forget to write it.

	private:
};