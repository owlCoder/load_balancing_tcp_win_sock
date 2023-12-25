# Load Balancing System

This is an implementation of a load balancing system based on the Round Robin algorithm. The system is designed to efficiently distribute client requests to available data processing threads.

## Characteristics

- Balances the load of incoming requests using the Round Robin algorithm
- Dynamically allocates worker threads for concurrent processing of requests
- Tracks the status of worker threads and distributes requests according to availability
- Supports configuring the number of worker threads and the maximum number of clients
- Uses a secure queue for data transfer between components

## Components

Main components of the system:

- Client Handler - manages client connections
- Thread Pool - dynamically allocates worker threads for processing
- Load Balancer - distributes client requests to workers
- Worker - a component that processes assigned requests

## Requests

- .NET environment
- Socket communication
- Multiplicity

## Usage

- Start the server application
- Start client instances to send requests
- Track statistics in the console for insight into performance
