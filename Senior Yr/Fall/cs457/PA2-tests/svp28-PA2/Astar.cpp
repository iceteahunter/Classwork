#include "Astar.h"



Astar::Astar(){

}

Astar::Astar(Puzzle initState){
	root.g =0;
	root.h = 0;
	root.state = initState;
	root.parent = NULL;
	//root.state.printBoard();
}

Astar::~Astar(){

}
vector<Move> Astar::printPath(Node* n){
	vector<Move> successMoves;
	vector<Move>::iterator it = successMoves.begin();
	int numMoves = 0;
	n->state.printBoard();

	for(int i =0; i < n->movesSoFar.size(); i++){
		// get pos 1
		int piece1 = n->movesSoFar[i].piece1;
		int piece2 = n->movesSoFar[i].piece2;
		cout << piece2 << " ";
		//cout << "Move " << n->state.board[pos1] << " to " << n->state.board[pos2] << endl;
		//n->movesSoFar[i].printMove();
		//cout << endl;

		numMoves++;
	}

	cout << endl;
	cout << "# of moves " << numMoves << endl;
	return n->movesSoFar;
	
}

void Astar::printPathRec(Node* n){
	if(n->parent != NULL){
		printPathRec(n->parent);
	}
	n->move.printMove();
}

vector<Move> Astar::Astar_search(){
	root.g = 0;
	root.h = heuristic(&root);
	
	opened.push(root);

	while(!opened.empty()){
		clock_t begin = clock();

		Node n2 = opened.top();
		Node* n = &n2;
	
		opened.pop();

	
		if(goalTest(n)){
			cout << "goal reached!" << endl;
			return printPath(n);
			
		}
		
		closed.push_back(&n2);

		vector<Move> allMoves = n->state.calculateMoves();

		for(int moveInx=0; moveInx < allMoves.size(); moveInx++){
			Node newChild(n);

			newChild.g = n->g + 1;
			newChild.depth = n->depth + 1;
			newChild.move = allMoves[moveInx];
			newChild.parent = n;
			newChild.movesSoFar = n->movesSoFar;

			newChild.addMove(allMoves[moveInx]);
			//n2.state.printBoard();
			newChild.state = n->state.applyMoveSeparate(allMoves[moveInx]);
			
			newChild.hash = newChild.state.computeHash();

			newChild.h = heuristic(&newChild);
			
			
			if( (!checkIfClosed(&newChild))){
				
				
				n->addChild(newChild);
				opened.push(newChild);
			}

			
		}

		clock_t end = clock();
		double elapsed = double(end-begin) / CLOCKS_PER_SEC;
		//cout << "elapsed = " << elapsed << endl;

		if(elapsed > 0.3){
			//sleep(5);
		}
	}
}

void Astar::printOpen(){
	while(!opened.empty()){
		Node x = opened.top();
		cout << " " << x.h + x.g;
		opened.pop();
	}
}

bool Astar::checkIfClosed(Node* child){
	
	


	for(int i=0; i<closed.size(); i++){
		if(closed[i]->hash == child->hash){
				//sleep(5);
				/*cout << "two hashs same" << endl;
				cout << "--------------------------" << endl;
				child->state.printBoard();
				closed[i].state.printBoard();
				cout << "--------------------------" << endl;
			sleep(0);
			*/
			if(closed[i]->equals(child)){
			
				return true;
			}
		}		
	}




	return false;
}

bool Astar::goalTest(Node* n){
	vector<int> tmpBoard = n->state.board;

	
	if(heuristic(n) == 0)
		return true;
	else
		return false;
	

	return true;
	
}

int Astar::heuristic(Node* a){
	int h = 0;
	Puzzle* tmp = &a->state;

	for(int i=0; i < tmp->getBoard().size() ; i++){
		h = h + distanceFromGoal(tmp->findAt(i), i, tmp->getK());
	}
	
	return h;
}

int Astar::distanceFromGoal(int piece, int pos, int k){

	int x1=0;
	int x2=0;
	int y1=0;
	int y2=0;

	//piece = piece - 1;
	if(piece == 0) {
		x1 = k-1;
		y1 = k-1;
	} else {
	  // X1 and Y1 is the position of the piece in the solved puzzle board
		x1 = piece % k;
		y1 = ((piece - x1) / k) + 1;  // which 'row' piece is on in the solved puzzle
	}

	// +1 to account for the 0 in array index
	pos = pos + 1;
	// same thing as doing piece == 0 check but for indices of arrayh
	if(pos == k*k){
		x2 = k-1;
		y2 = k-1;
	} else {
		// X2 and Y2 is the position of piece in the current puzzle state
		x2 = pos % k;
		y2 = ((pos - x2) / k) + 1;
	}


	// use the variation of distance formula
	int dist = abs(x1 - x2) + abs(y1 - y2);

	/*if(piece < 15){
		cout << x1 << " " << x2 << " " << y1 << " " << y2 << endl;
		cout << "piece=" << piece << " pos= " << pos << " dist=" << dist << endl;

		sleep(1);
	}*/

	// if i calulated this correctly, it should be the manhattan distance 
	return dist;

}
