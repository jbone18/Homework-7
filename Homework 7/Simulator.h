#pragma once
#ifndef SIMULATE_H_
#define SIMULATE_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <ios>
#include "LandingQueue.h"
#include "ServiceQueue.h"
#include "DepartureQueue.h"
#include "Random.h"

Random my_random;  // Global variable 

class Simulator
{
private:
	int total_time;  // total time to run simulation
	int clock;       // current time
	int num_gates;	//  Number of gates in the airport

	// landing queue --> service queue --> departure queue
	LandingQueue *landing_queue;
//	ServiceQueue *service_queue;
	DepartureQueue *departure_queue;

	std::vector<ServiceQueue *> gates;

	// Remember me!
	int read_int(const std::string &prompt, int low, int high)
	{
		if (low >= high) // invalid range
			throw std::invalid_argument("invalid range specified");

		std::cin.exceptions(std::ios_base::failbit);
		int num = 0;
		while (true) 
		{
			try 
			{
				while (true) 
				{
					std::cout << prompt;
					std::cin >> num;
					if (num >= low && num <= high) 
					{ 
						// within the specified range
						std::cout << std::endl;
						return num;
					}
				}
			}
			catch (std::ios_base::failure) 
			{
				std::cout << "Bad numeric string -- try again\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			}
		}
	}
public:
	Simulator() 
	{
		landing_queue = new LandingQueue();
		//service_queue = new ServiceQueue();
		departure_queue = new DepartureQueue();
	}
	void enter_data()
	{
		std::cout << "Welcome CS273 to the Simple Airport Simulator.\n\n";
		std::cout << "We are CS273ville, a small but feisty airport!\n";
		std::cout << "Remember to re-read section 6.5 of the text book\n\n";

		int rate = read_int("Please enter the plane arrival rate (planes/hour): ", 1, 59);
		double arrival_rate = rate / 60.0;

		int min_service = read_int("Please enter the minimum service time (mins): ", 0, INT_MAX);
		int max_service = read_int("Please enter the maximum service time (mins): ", 0, INT_MAX);
		int departure_time = read_int("Please enter the departure time (mins): ", 1, INT_MAX);

		num_gates = read_int("Please enter the number of gates: ", 0, INT_MAX);

		for (int i = 0; i < num_gates; i++)
		{
			gates.push_back(new ServiceQueue());
		}

		total_time = read_int("Please enter the simulation time (hours): ", 1, INT_MAX);
		total_time *= 60;

		// set the arrival_rate for the landing queue
		landing_queue->set_arrival_rate(arrival_rate);

		for (int i = 0; i < num_gates; i++)
		{
			// set the service times for the service queue 
			gates[i]->set_service_times(min_service, max_service);

			//  sets the number of gates
			gates[i]->set_num_gates(num_gates);

			// pass references to the landing and departure queue to the service queue
			gates[i]->set_landing_queue(landing_queue);
			gates[i]->set_departure_queue(departure_queue);

		}
		
		// set the departure time for the departure queue
		departure_queue->set_departure_time(departure_time);
	}
	void run_simulation()
	{
		// run the simulation
		for (clock = 0; clock < total_time; ++clock)
		{
			// for each time interval ...
			landing_queue->update(clock);
		
			//  loop through each gate and use the update function
			for (int i = 0; i < num_gates; i++)
			{
				gates[i]->update(clock);
			}

			departure_queue->update(clock);
		}
	}





	void show_stats()
	{
		std::cout << "Number of planes served in the arrival queue: " << landing_queue->get_num_served() << std::endl;
		std::cout << "Total wait time for all planes in arrival queue: " << landing_queue->get_total_wait() << " minutes. " << std::endl;
		// FIXME: Calculate and display the average wait time for the landing queue (per minute)

		int total_served_landing = landing_queue->get_num_served();
		int total_wait_landing = landing_queue->get_total_wait();

		int average_wait_landing = total_wait_landing / total_served_landing;

		std::cout << "The average wait time for the landing queue is " << average_wait_landing << " minutes. " << std::endl;





		std::cout << std::endl;
		std::cout << "Number of planes served in the departure queue: " << departure_queue->get_num_served() << std::endl;
		std::cout << "Total wait time for all planes in departure queue: " << departure_queue->get_total_wait() << " minutes. " << std::endl;
		// FIXME: Calculate and display the average wait time for the departure queue

		int total_served_departure = departure_queue->get_num_served();
		int total_wait_departure = departure_queue->get_total_wait();

		int average_wait_departure = total_wait_departure / total_served_departure;

		std::cout << "The average wait time for the departure queue is " << average_wait_departure << " minutes. " << std::endl;

		std::cout << std::endl;
	}
};
#endif