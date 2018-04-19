#pragma once
#ifndef SERVICE_Q_H_
#define SERVICE_Q_H_

#include <queue>
#include "Plane.h"
#include "LandingQueue.h"
#include "DepartureQueue.h"
#include "Random.h"

extern Random my_random; // Enables us to access the global variable declared in Simulator.h

/*The service queue takes a plane from the landing queue and adds it to its queue.
  When a plane in service queue has finished being serviced, it will be placed in the departure queue. */
class ServiceQueue
{
private:
	int min_service_time, max_service_time;  // range of service times
	LandingQueue *landing_queue;             // pointer to the landing queue
	DepartureQueue *departure_queue;         // pointer to the departure queue
	std::queue<Plane *> the_queue;           // queue of planes (just ONE) in the service queue
	int num_gates;
public:
	ServiceQueue() {}

	void set_service_times(int min_service_time, int max_service_time) 
	{
		this->min_service_time = min_service_time;
		this->max_service_time = max_service_time;
	}
	void set_landing_queue(LandingQueue *landing_queue) { this->landing_queue = landing_queue; }
	void set_departure_queue(DepartureQueue *departure_queue) { this->departure_queue = departure_queue; }
	void set_num_gates(int num_gates) { this->num_gates = num_gates; }

	void update(int clock)
	{
		// there is a plane at the gate
		if (!the_queue.empty()) 
		{

			Plane *plane = the_queue.front();

			// check if a plane is ready to move from the service queue to the departure queue 
			if ((clock - plane->start_service_time) > plane->service_time) 
			{
				// Remove the plane from the service queue
				the_queue.pop();
			
				// Update the enter_departure_time attribute for the plane

				int time = clock;
				plane->enter_departure_time = time;

				//Add the plane to the Departure Queue queue (pointer)
				departure_queue->the_queue.push(plane);
			}
		}

		// the gate is empty - ready to serve!
		if (the_queue.empty()) 
		{
			// move a plane from the landing queue to the service queue
			if (!landing_queue->the_queue.empty()) 
			{
				//Create a new plane at the front of the queue
				Plane *plane = landing_queue->the_queue.front();

				//Remove the previous plane from the queue.
				landing_queue->the_queue.pop();

				//Landing queue time begins
				int time = clock;

				//Landing queue time ends
				int arrival = plane->arrival_time;
	
				//Time plane waited in the landing queue
				int wait_time_landing = time - arrival;

				
				// update total_wait and num_served for the landing queue
				++(landing_queue->num_served);
				(landing_queue->total_wait) += wait_time_landing;

				// Update the start_service_time attribute for the plane
				int _time = clock;
				plane->start_service_time = _time;


				/*HINT: You can use my_random.next_int(n) to get a random value between 0 and n.  This will help
				  you determine a random number within the range of service times. */
				//Num is given a value inside the desired min-to-max range
				int num = max_service_time - min_service_time;

				//Finds a random number from 0 to num
				int random = my_random.next_int(num);
				
				//Gives a random number from the addition of two variables. min_number <= service_time <= max_number
				int service_time = random + min_service_time;

				//Assigns the calculate value into the service time.
				plane->service_time = service_time;

				// Add the plane to the service queue
				the_queue.push(plane);
			}
		}
	}
};
#endif