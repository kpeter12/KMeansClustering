//Kevin Peterson
//OLA4
//KMeans
//This program performs K means clustering algorithm to a set of points
//given by the training data and then outputs the number classified correctly
//based on the testing data.
#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

//prototypes
double randDouble(double min, double max);
double euclidianDistance(vector<double> p1, vector<double> p2);
double avgColumn(vector<vector<double> > vec, int col);
int randInt(int min, int max); //returns random int between min and max


int main(int argc, char * argv[])
{
	
	int randomSeed = atoi(argv[1]);          //the random seed
	int numClusters = atoi(argv[2]);         //number of clusters the algorithm wil seperate the data into
	int numFeatures = atoi(argv[3]);         //number of real valued features in the data set
	string trainingFile = argv[4];           //training file for the algorithm
	string testingFile = argv[5];            //testing file for the algorithm
	ifstream inFile;
	
	vector<vector<double> > trainingData;
	vector<vector<double> > testingData;
	vector<vector<double> > centroids;
	vector<int> correspondingCentroids;            //corresponding vector  to 'trainingData' that holds centroids for the trainingData
	vector<double> trainingDataClassLabels;        //corresponding vector to 'trainingData' that holds the class labels for the training data
	vector<double> testingDataClassLabels;        //corresponding vector to 'trainingData' that holds the class labels for the training data
	vector<double> centroidClassLabels(numClusters,-1);            //corresponding vector to 'centroids', that holds that class labels for each centroid
	
	
	srand(randomSeed);
	
	double value;
	string line;
	//Read in training data from txt file
	inFile.open(trainingFile);
    getline(inFile,line);
    stringstream parsed(line);

    // Prep vectors...
	int j = 0;
    while (!inFile.eof()) {
        trainingData.push_back(vector<double>());
		correspondingCentroids.push_back(-1);          //initialize entire corresponding centroids to centroid 0
		
		
		for (int i = 0; i < numFeatures; i++)
		{
			parsed >> value;
			trainingData.at(j).push_back(value);
		}
		parsed >> value;
		trainingDataClassLabels.push_back(value);
		getline(inFile,line);
		parsed.clear();
		parsed.str(line);
		j++;
		
    }
	inFile.close();
	
	//read in data for testing file
	inFile.open(testingFile);
    getline(inFile,line);
    parsed.clear();
	parsed.str(line);

    // Prep vectors...
	j = 0;
    while (!inFile.eof()) {
        testingData.push_back(vector<double>());
		for (int i = 0; i < numFeatures; i++)
		{
			parsed >> value;
			testingData.at(j).push_back(value);
		}
		parsed >> value;
		testingDataClassLabels.push_back(value);
		getline(inFile,line);
		parsed.clear();
		parsed.str(line);
		j++;
    }
	inFile.close();
	
	//cout << "Starting Centroid Values: " << endl;
	//hold used centroids in a vector so we don't reuse the same one over and over again
	vector<bool> startingCentroidsTaken(trainingData.size(), false);
	for (int i = 0; i < numClusters; i++)
	{
		centroids.push_back(vector<double>());
		int randStart = randInt(0, trainingData.size());
		while (startingCentroidsTaken.at(randStart) == true)        //keep generating new start until we get a centroid that we haven't used yet
		{
			randStart = randInt(0, trainingData.size()); 
		}
		startingCentroidsTaken.at(randStart) = true;
		for (int j = 0; j < numFeatures; j++)
		{
			centroids.at(i).push_back(trainingData.at(randStart).at(j));    //(avgColumn(trainingData, j) * randDouble(.97,1.03));
			//cout << centroids.at(i).at(j) << " ";
		}
		//cout << endl;
	}
	
	
	bool valueChanged = true;             //
	while (valueChanged)
	{
		valueChanged = false;
		//assign closest centroid to every point in the training data vectors
		for (int i = 0; i < trainingData.size(); i++)
		{
			int closestCentroid = -1;
			double closestDistance = 9999999;
			for (int j = 0; j < centroids.size();j++)
			{
				double distance = euclidianDistance(trainingData.at(i),centroids.at(j));
				if (distance < closestDistance){
					
					closestDistance = distance;
					closestCentroid = j;
				}
			}
			if (correspondingCentroids.at(i) != closestCentroid)
			{
				valueChanged = true;
			}
			correspondingCentroids.at(i) = closestCentroid;
			//cout << "Closest centroid for point " << i << ": " << closestCentroid << endl;

		}
		
		//reassign centroid values
		for (int i = 0; i < centroids.size(); i++)
		{
			vector<double> tempCentroid(centroids.at(0).size(),0);   //will hold the sum value of all the centroid coordinates
			double numOccurences = 0;   //will be what we divide by to find the new average
			for (int j = 0; j < trainingData.size(); j++)
			{
				if (correspondingCentroids.at(j) == i)
				{
					numOccurences++;
					for (int k = 0; k < tempCentroid.size(); k++)
					{
						tempCentroid.at(k) += trainingData.at(j).at(k);
					}
				}
			}
			for (int j = 0; j < tempCentroid.size(); j++)
			{
				centroids.at(i).at(j) = tempCentroid.at(j) / double(numOccurences);
			}
		}
		
		//Starting training data vector and corresponding centroids
		
		/*
		cout << "Training Vector and corresponding centroids" << endl;
		for (int i = 0; i < trainingData.size(); i++)
		{
			for (int j = 0; j < numFeatures; j++)
			{
				cout << trainingData.at(i).at(j) << " ";
			}
			cout << "   class: "<<trainingDataClassLabels.at(i);
			cout << "   centroid: "<< correspondingCentroids.at(i) << endl;
			
		}
		*/
		/*
		cout << "New centroid values: " << endl;
		for (int i = 0; i < centroids.size(); i++)
		{
			for (int j = 0; j < centroids.at(i).size(); j++)
			{
				cout << centroids.at(i).at(j) << " ";
			}
			cout << endl;
		}
		*/
	}
	
	//centroid vectors have been found and all points
	//in training data have been assigned a centroid vector
	//now we must assign each centroid a class label
	
	for (int i = 0; i < centroids.size(); i++)
	{
		vector<int> classCounts(trainingDataClassLabels.size(), 0);      //holds the num occurrences a class at each centroid value, using trainingDataClassLabels.size()
		for (int j = 0; j < trainingDataClassLabels.size(); j++)          //assumes that the class label won't be above the number of items we are using which is true for cancer and iris
		{
			if (int(correspondingCentroids.at(j)) == i)
			{
				//if (trainingDataClassLabels.at(j) < centroids.size())       //so we don't get seg fault
					classCounts.at(trainingDataClassLabels.at(j))++;
			}
		}
		//cout << "centroid "<< i << " occurences: ";
		int maxCount = -1;
		for (int c = 0; c < classCounts.size(); c++)
		{
			//cout << classCounts.at(c) << " ";
			if (classCounts.at(c) > maxCount)
			{
				maxCount = classCounts.at(c);
				centroidClassLabels.at(i) = c;
			}
		}
		//cout << endl;
	}
	//debug print centroid class labels
	//cout << "Centroid class labels: ";
	for (int c = 0; c < centroidClassLabels.size(); c++)
	{
		//cout << centroidClassLabels.at(c) << " ";
	}
	//cout << endl;
	
	
	//debug print testing data
	for (int i = 0; i < testingData.size(); i++)
	{
		for (int j = 0; j < testingData.at(i).size(); j++)
		{
			//cout << testingData.at(i).at(j) << " ";
		}
		//cout << "  "<< testingDataClassLabels.at(i);
		//cout << endl;
	}
	int correctClassifications = 0;
	//cout << "correctly classified: ";
	for (int i = 0; i < testingData.size(); i++)
	{
		double shortestDistance = 9999999;
		int closestCentroid = -1;
		for (int j = 0; j < centroids.size();j++)
		{
			double tempDistance = euclidianDistance(testingData.at(i), centroids.at(j));
			if (tempDistance < shortestDistance)
			{
				shortestDistance = tempDistance;
				closestCentroid = j;
			}
		}
		if (int(centroidClassLabels.at(closestCentroid)) == int(testingDataClassLabels.at(i)))
		{
			correctClassifications++;
			//cout << i << " ";
		}
	}
	
	cout << correctClassifications << endl;
 	return 0;
}

//returns a random double between min and max
double randDouble(double min, double max)
{
    double val = (double)rand() / RAND_MAX;
    return min + val * (max - min);
}

//returns euclidian distance between p1 and p2. both vectors
//need to be the same length
double euclidianDistance(vector<double> p1, vector<double> p2)
{
	double sum = 0;
	for (int i = 0; i < p1.size(); i++)
	{
		sum +=  pow((p1.at(i) - p2.at(i)),2);
	}
	return sqrt(sum);
}

//finds the average of all the values in col of vec and returns that double
double avgColumn(vector<vector<double> > vec, int col)
{
	double sum = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		sum += vec.at(i).at(col);
	}
	sum /= double(vec.size());
	return sum;
}

int randInt(int min, int max) //returns random int between min and max
{

	int random = rand() % max + min;
	return random;
}

//finds the average of all the values in col of vec and returns that double
int maxColumn(vector<vector<double> > vec, int col)
{
	double max = -9999;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec.at(i).at(col) > max)
			max= vec.at(i).at(col);
	}
	return max;
}
