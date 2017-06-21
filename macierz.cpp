#include <thread>
#include <iostream>
#include <ncurses.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <queue>
#include <string>


#include <assert.h>



std::queue <int> row1;
std::mutex row1Guard;

std::queue <int> row2;
std::mutex row2Guard;

static int a = 5;
static int b = 5;
int tab[100][100] = {};
std::mutex tabGuard;
std::queue <int> tabtemp;

std::mutex displayGuard;



void producer(){
	while (true){

	row1Guard.lock();
	row2Guard.lock();
	displayGuard.lock();


	row1.push(std::rand() %a);
	row2.push(std::rand() %b);

	row1Guard.unlock();
        row2Guard.unlock();
        displayGuard.unlock();

	
	 std::this_thread::sleep_for(std::chrono::seconds(1));
	

	}
}

void transformer(){
	while (true){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	row1Guard.lock();
        row2Guard.lock();
	displayGuard.lock();
	tabGuard.lock();
	for(int i = 0 ; i<a; i++ ){
		for(int j = 0 ; j<b; j++ ){
		tabtemp.push(tab[i][j]);
		}
   	}
	tabtemp.push(0);
	tabtemp.pop();
	
	 for(int i = 0 ; i<a; i++ ){
                for(int j = 0 ; j<b; j++ ){
                tab[i][j]=tabtemp.front();
		tabtemp.pop();
                }
        }

	if( !row1.empty() && !row2.empty())
	{
	tab[row1.front()][row2.front()]=1;
	row1.pop();
	row2.pop();		
        }	

	
        row1Guard.unlock();
        row2Guard.unlock();
        displayGuard.unlock();
	tabGuard.unlock();



	}


}

void display(){
	initscr();
	while(true){


  std::this_thread::sleep_for(std::chrono::seconds(1));



  clear();
for(int i=0; i < a; i++){

    for(int j=0; j < b; j++){
	
        printw("%d",tab[i][j]);
    }
    printw("\n");

refresh();

}	


}
endwin();
}







int main(){

srand(time(NULL));

auto producerThread = std::thread(producer);
auto transformerThread = std::thread(transformer);
auto displayThread = std::thread(display);
producerThread.join();
transformerThread.join();
displayThread.join();

return 0;
}
