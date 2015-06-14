#Rapport de stage

##Introduction

Welcome to the Phoenix team. I am Tanguy Pedrazzoli, working for Orange Labs in Cesson-Sévigné. You are going to work on the solution for the future TV box of Orange, that will be released in 18 month from now. As can show the project name, we have decided to rewrite from scratch the software, going from a single webapp system to a multi C++ application system, making the whole thing a lot more maintainable and efficient. The details will be explained later, because I am going to present you Orange labs, the team, and the way people are working here.

###Glossary

* Data Model : List of all the URI (local or web) developers needs to retrieve various informations.

* Middleware : List of APIs that allows to abstract the drivers and to decode the video streams. It implements the data model too.

* EPG : Stands for "Electronic Program Guide". Informations we retrieve from particular server that gives us all the informations about a channel and its programs (name, language, duration etc).


##Presentation of Orange - Team - Working method

###Orange Labs

Orange Labs is the Research and development department of Orange, former known as "France Télécom R&D". The goal of Orange Labs is to
work on making a discovery that will lead to a new product. Actually, the company counts 5'000 employees all around the world. There is eight laboratory in France (Issy-les-Moulineaux, Rennes, Grenoble, Sophia-Antipolis, La Turbie, Lannion, Belfort and Caen) and eleven abroad (London, Madrid, Tokyo, Pekin etc). The local director (in Rennes) is Christine Marcatte and the head of the SOFT department is Jean-Luc Beraudo de Pralormo. The Phoenix team leader is Frederic Garde and the Scrum master is Michel Train.
Orange labs have a lot of partnership, but the most importants are the one with Microsoft and the other with Bull around the ObjectWeb 2 association.

####The Team

You will work with a team composed of ten engineers and one scrum master. One manager will cover up the evolution of the project and one architect will give all the informations concerning the hardware and middleware. The team is split into two "mêlées", each "mêlée" working on a different app. The goal of the second one is not to proceed in parallel, but to start the other app, so the other "mêlée" will not lose time switching to the next app.

####The working method

The developers team uses the scrum agile method, running one-month sprints. Each stories are made of tasks that are reviewed two times before being merged into the master branch on the Git system. Once the stories are defined, the first task is to design the new features that will be added. Everybody is invite to criticize the design to improve it. The design done, someone fill the documentation and the task are assigned. Each morning, the team does a scrum meeting to talk about their progress, problem or bug discovery. Each morning you have to tell the number of hours you have worked on your task, and estimate the remaining hours. This step is very important, as the scrum master base his schedule on these estimations.

####The working environment

You will have to make an application for the Orange Box TV service. The OS ported on this box is an heavily modified Linux, so even if you have the choice of the OS, you should work with a Ubuntu or another Debian-based distribution. Moreover, everybody in the team is using Ubuntu, and all the guides in the Phoenix wiki are tested on Debian.

##The project

####I. Recreating the software from scratch

The TV by Orange offers TV in live obviously, but offers VOD, a Web browser, possibility to subscribe to particular channel package and an EPG application.

The original software, that is actually in production on the Box TV, is an single Web app in standard combo HTML5 - CSS3 - JavaScript. Along with performance problems, this solution is hardly maintainable (due to a lack of modularity) and it is nearly impossible to improve and to add new features in a fast and efficient way. So Frederic Garde decided to give a C++ / Qt solution a shot. The aim is to optimize the hardware usage by using a multi application design, and to make the coding time a lot lesser while developing a new feature.

####II. Qt, an API and a Framework (Kind of)



####III. The EPG application

The EPG application aims to give people the complete program schedule, per channel, day, prime time or genre. The main goal is to replace all the others dedicated support (especially magazine). The other goal is to become independent of EIT system, and its lack of modularity (as it gives only informations about the current and next program), and being capable of implementing features, as browsing seven days of programs. The EPG application is composed of four menus. The first one is the "Now" menu, and its goal is to show the actual program playing on each channels. Two others menus, named "First Prime time" and "Second Prime time", will show the different program playing on the first prime (20h45) and second prime (22h30). The last one is the "Grid" menu, and here, the user will be able to browse from now to seven days in the future all the programs that are going to be played.

We retrieve the data need by requesting the TVM server. They will give a JSON file. On this request, we can specify the date we are looking for, the period (if we want the full day, only three hours or a specific prime) or a specific channel. Even if TVM has developed different algorithms on their back end to implement these filters, the EPG application is requesting for the full day, creating the objects, and the filters will be processed directly by the application, as it is way faster than doing several requests and recreating new objects on each requests.

Even if this is a classic application with a GUI and models, we don't develop it following the MVC pattern. In fact, we use the hexagonal pattern. The processing is done by the "business" and the view, network, DBus etc are connected to the business by interfaces. We call them the connectors. Using this pattern allows the business to be perfectly independent of his connectors (the business should run without any connectors), and one connector are unaware of the others, reducing the dependencies to the maximum. The business and its connectors are linked by a specific design-pattern, the observer-observable. This way, we can add as many connectors as possible, without adding any line of code in the business. For the view for example, the business will notify the view that the EPG data has been updated, and therefore the view will request the business to get the data. This helps to separate the development and allows parallel coding, and add a level of abstraction.

The hardest thing to do with this application is to test correctly all the possible scenarii of usage. Tests must cover the maximum of code percentage (80% is ideal, as certain scenarii are system dependent), to be able to detect more bugs before putting it in production. Unit tests are 50% of the coding time, as they are really important. The code coverage is enable in gcc-g++ with the option --coverage, that will export an html file. Optimizing the EPG app is another great challenge. As the hardware is not that powerful, and as we handle several megabytes of JSON data, we must be careful about memory usage and the processing time. A good mix between local processing and request to the server as been found, as we first request a day, then apply filter on it and then, if a user is on the Grid menu, we request the next day. We could have requested seven day in one time, but as we don't know the bandwidth available, it is not a viable solution. Requesting for each menu is possible, because TVM offers the needed APIs, but we cannot test the server under heavy charge at the moment, and Orange cannot afford to put something in production that can bug when more and more people would get the new software.
