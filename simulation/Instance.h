/**
 * @file Instance.h
 * @brief TSP data
 *
 */

#ifndef INSTANCE_H
#define INSTANCE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string.h>

using namespace std;

typedef struct node {
    double x;
    double y;
} Node;

/**
 * Class that describes a TSP instance (a cost matrix, nodes are identified by integer 0 ... n-1)
 */
class Instance
{
    public:
        int nodes_card;
        double x_bound;
        double y_bound;
        int start_index;
        vector< vector<double> > costs;
        vector<Node> nodes;

        Instance(int n, int a, int b, int si, int e) : nodes_card(n), x_bound(a), y_bound(b), start_index(si) {
            srand(time(NULL));

            // build nodes and costs matrix
//            ifstream ifs;
//            ifs.open("graph.txt", std::ifstream::in);
//
//            char nodes[256000];
//            ifs.getline(nodes, 256000);
//
//            size_t tpos = 0;
//            string tokens(nodes);
//
//            while((tpos = tokens.find(" ")) != string::npos) {
//
//                vector<string> coords;
//
//                Node node;
//                size_t pos = 0;
//                string token = tokens.substr(0, tpos);
//
//                while ((pos = token.find(";")) != string::npos) {
//                    coords.push_back(token.substr(0, pos));
//                    token.erase(0, pos + 1);
//                }
//                node.x = stod(*coords.begin());
//                node.y = stod(*(coords.end()-1));
//              
//                this->nodes.push_back(node);
//
//                tokens.erase(0, tpos + 1);
//            }
//

            if (e == 0) {

                for (int i = 0; i < nodes_card; i++) {
                    Node node;
                    node.x = fRand(0.0, x_bound);
                    node.y = fRand(0.0, y_bound);

                    this->nodes.push_back(node);
                }

            }
            else if (e == 1) {

                for (int i = 0; i < nodes_card; i++) {
                    Node node;
                    node.x = fRand(0.0, x_bound/50);
                    node.y = fRand(0.0, y_bound/2);

                    this->nodes.push_back(node);
                }
            }
            else if (e == 2) {

                int xedge = x_bound/4;
                int yedge = x_bound/4;
                uint nc = nodes_card;

                while (nodes.size() <= nc / 4) {
                    Node node;
                    node.x = fRand(xedge/2, xedge+xedge/2);
                    node.y = fRand(y_bound/2+yedge/2, y_bound - yedge/2);
                    this->nodes.push_back(node);
                }

                while (nodes.size() <= nc / 2) {
                    Node node;
                    node.x = fRand(x_bound/2+xedge/2, x_bound-xedge/2);
                    node.y = fRand(y_bound/2+yedge/2, y_bound - yedge/2);
                    this->nodes.push_back(node);
                }

                while (nodes.size() <= (nc / 4 * 3)) {
                    Node node;
                    node.x = fRand(xedge/2, xedge+xedge/2);
                    node.y = fRand(yedge/2, yedge+yedge/2);
                    this->nodes.push_back(node);
                }

                while (nodes.size() < nc) {
                    Node node;
                    node.x = fRand(x_bound/2+xedge/2, x_bound-xedge/2);
                    node.y = fRand(yedge/2, yedge+yedge/2);
                    this->nodes.push_back(node);
                }
            }
            else if (e == 3) {
                int radius = 399;
                Node center;
                center.x = 0;// x_bound / 2;
                center.y = 0; //y_bound / 2;
                for (int i = 0; i < nodes_card; i++) {
                    Node node;
                    node.y = -1;
                    while (node.y < 0) {
                        node.x = fRand(-x_bound/2, x_bound/2);
                        double d = pow(radius, 2) - pow((node.x), 2);

                        if (node.x > 0)
                            node.y = sqrt(d);
                        else
                            node.y = -sqrt(d);

                        if (d == 0)
                            node.y = center.y;
                    }
                    this->nodes.push_back(node);
                }

            }
            else if (e == 4) {
                for (int i = 0; i < nodes_card; i++) {
                    Node node;
                    node.x = fRand(0.0, x_bound);
                    node.y = fRand(0.0, y_bound);

                    this->nodes.push_back(node);
                }
            }

            costs.reserve(nodes_card);

            // calculate euclidean distances

            costs.resize(n);
            for (int i = 0; i < nodes_card; ++i)
               costs[i].resize(nodes_card);

//            char cost_line[256000];
  //          char* cost_token;
            for(int i = 0; i < nodes_card; i++) {

   //             ifs.getline(cost_line, 256000);
     //           cost_token = strtok(cost_line, " ");

                for(int j = 0; j < nodes_card; j++) {
      //              costs[i][j] = std::stod(cost_token);

                    //cout << costs[i][j] << " ";

                    Node fnode = this->nodes[i];
                    Node snode = this->nodes[j];
                    double cost = sqrt( pow((snode.x-fnode.x), 2) + pow((snode.y-fnode.y), 2) );

                    costs[i][j] = cost;                                                                                                           
                    costs[j][i] = cost;

                    if (i == j)
                        costs[i][j] = 0.0;


                //    cost_token = strtok(NULL, " ");
                }
                //cout << "\n";

            }


//            for (int i = 0; i < nodes_card; i++) {
//                vector<double> row;
//                for (int j = 0; j < nodes_card; j++) {
//                    Node fnode = this->nodes[i];
//                    //cout << "node " << i << ": (" << fnode.x << "," << fnode.y << ")" << endl;
//                    Node snode = this->nodes[j];
//                    //cout << "node " << j << ": (" << snode.x << "," << snode.y << ")" << endl;
//
//                    //somma dei quadrati delle diff tra le coord
//
//                    row.push_back(cost);
//                    //cout << "cost: " << cost << endl;
//                }
//                this->costs.push_back(row);
//            }

//                // save data to CSV file
//                ofstream ofs;
//                ofs.open("graph.txt", ofstream::out | ofstream::app);
//
//                for(uint i = 0; i < this->nodes.size(); i++) {
//                    ofs << this->nodes[i].x;
//                    ofs << ";";
//                    ofs << this->nodes[i].y;
//                    ofs << " ";
//                }
//                ofs << "\n";
//
//                for(int i = 0; i < nodes_card; i++) {
//                    for(int j = 0; j < nodes_card; j++) {
//                        ofs << costs[i][j];
//                        ofs << " ";
//                    }
//                    ofs << "\n";
//                }
//
//                ofs.close();
//            ifs.close();

            cout << "[";
            for(vector<Node>::iterator n = this->nodes.begin(); n != this->nodes.end(); n++)
                cout << n->x << ",";
            cout << "]\n";

            cout << "[";
            for(vector<Node>::iterator n = this->nodes.begin(); n != this->nodes.end(); n++)
                cout << n->y << ",";
            cout << "]\n";


        }

        double fRand(double fMin, double fMax) {
            double f = (double)rand() / RAND_MAX;
            return fMin + f * (fMax - fMin);
        }
};

#endif /*  INSTANCE_H */

