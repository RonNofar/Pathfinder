#include <iostream> // std::cout, ::endl, ::cin
#include <iomanip> // std::setw, ::setfill
#include <fstream> // std::ifstream, ::ios_base
#include "Graph.h"
#include "Tools.h"

int main() {
	/*
	std::ifstream readFile;
	readFile.open("input.txt", std::ios_base::in);

	Graph graph(readFile);
	for (int i = 0; i < graph.numberOfVertex(); ++i) {
		graph.dijsktra(i);
	}
	readFile.close();

	//Graph graph(3, 5, 6);
	*/
	std::cout << "WELCOME TO PATHFINDER v1.0" << std::endl 
		<< std::setfill('=') << std::setw(26) << "=" << std::endl;
	for (;;) {
		std::string cmd;
		std::cout << "Would you like to (L)oad an existing input file, create a (R)andom input, or e(X)it?" << std::endl
			<< ">>> ";
		if (std::cin >> cmd) {
			cmd = tolower(cmd[0]); // allows for both lower and upper case inputs
			if (cmd[0] == 'l') {
				bool load = true;
				std::cout << std::endl << "Loading..." << std::endl << std::endl;
				std::ifstream readFile;
				readFile.open("input.txt", std::ios_base::in);

				Graph graph(readFile);
				int vNum = graph.numberOfVertex()-1;
				std::cout << "Graph successfully loaded." << std::endl << std::endl;
				while (load) {
					std::cout << "Pick a source between 0 and " << vNum << ": " << std::endl << ">>> ";
					int cmd;
					if (std::cin >> cmd) {
						if (cmd <= vNum) {
							graph.dijsktra(cmd);
							for (;;) {
								std::cout << std::endl << "Pick a another source? (Y/N)" << std::endl << ">>> ";
								std::string cmd;
								std::cin >> cmd;
								cmd = tolower(cmd[0]);
								if (cmd[0] == 'y') {
									break;
								}
								else if (cmd[0] == 'n') {
									load = false;
									std::cout << std::endl << std::endl;
									break;
								}
								else {
									InvalidInput();
								}
							}
						}
						else {
							InvalidInput();
						}
					}
					else {
						InvalidInput();
					}
				}
				
			}
			else if (cmd[0] == 'r') {
				bool random = true;
				while (random) {
					double d;
					std::cout << std::endl << "Density: " << std::endl << ">>> ";
					if (std::cin >> d) {
						if (d <= 0) {
							InvalidInput();
						}
						else {
							while (random) {
								double m;
								std::cout << std::endl << "Max range (weight): " << std::endl << ">>> ";
								if (std::cin >> m) {
									if (m <= 0) {
										InvalidInput();
									}
									else {
										while (random) {
											int v;
											std::cout << std::endl << "Number of vertices: " << std::endl << ">>> ";
											if (std::cin >> v) {
												if (m <= 0) {
													InvalidInput();
												}
												else {
													Graph graph(d, m, v);
													int vNum = graph.numberOfVertex() - 1;
													std::cout << std::endl << "Graph successfully created." << std::endl << std::endl;
													while (random) {
														std::cout << "Pick a source between 0 and " << vNum << ": " << std::endl << ">>> ";
														int cmd;
														if (std::cin >> cmd) {
															if (cmd <= vNum) {
																graph.dijsktra(cmd);
																for (;;) {
																	std::cout << std::endl << "Pick a another source? (Y/N)" << std::endl << ">>> ";
																	std::string cmd;
																	std::cin >> cmd;
																	cmd = tolower(cmd[0]);
																	if (cmd[0] == 'y') {
																		break;
																	}
																	else if (cmd[0] == 'n') {
																		random = false;
																		std::cout << std::endl << std::endl;
																		break;
																	}
																	else {
																		InvalidInput();
																	}
																}
															}
															else {
																InvalidInput();
															}
														}
														else {
															InvalidInput();
														}
													}
												}
											}
											else {
												InvalidInput();
											}
										}
									}
								}
								else {
									InvalidInput();
								}
							}
						}
					}
					else {
						InvalidInput();
					}
				}


			}
			else if (cmd[0] == 'x') {
				std::cout << std::endl << "Exiting..." << std::endl
					<< "Thank you for trying Pathfinder!" << std::endl << std::endl;
				break;
			}
			else {
				InvalidInput();
			}
		}
	}
	return 0;
}