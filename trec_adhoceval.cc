/* @file trec_adhoceval.cc
 * @abstract Implementation file for TREC Evaluation which read result file and evaluate for Adhoc Retrieval.
 *
 * Copyright 2012 Gaurav Arora
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 19  */
#include "trec_adhoceval.h"
#include <iostream>
#include <fstream>
#include "split.h"

using namespace std;

void 
AdhocEvaluation::intialise() {

	maxNumberRetrieved = configobj.get_noresults();
	precisionAtRank.clear();
	precisionAtRecall.clear();
	numberofEffQuery = 0;
	totalNumberofRetrieved = 0;
	totalNumberofRelevant = 0;
	meanAveragePrecision = 0; 
}


void
AdhocEvaluation::evaluate() {
intialise();
int effQueryCounter = 0;
set<int>  numberofRelevantRetrived ;
set<int> numberofRelevant  ;
set<int>  numberofRetrived ;

vector<vector<Record> > listofRetrieved;
vector<vector<Record> > listofRelevantRetrieved;
vector<int> vecNumberofRelevant;
vector<int> vecNumberofRetrieved;
vector<int> vecNumberofRelevantRetrieved;
vector<string> vecQueryNo;
ifstream resultfile;
string resultfilename = configobj.get_resultsfile();
//string resultfilename = "/home/gaurav/Work/xapiantrec/testresult1";
//cout<<resultfilename<<endl;
resultfile.open(resultfilename.c_str(),ifstream::in);
if(!resultfile.is_open()) {
	cout<<"ERROR - can't open the result file"<<configobj.get_resultsfile().c_str() << "for reading"<<endl;
}
string str;
string previous = "";
int numberofRetrievedCounter = 0;
int numberofRelevantRetrievedCounter = 0;
vector<Record> *relevantRetrieved = new vector<Record>();
vector<Record> *retrieved = new vector<Record>();
while( resultfile.good() && !resultfile.eof() ) {
getline(resultfile,str);
vector<string> data;
split(str,' ',data);
if (data.size() == 7) {
	//cout<<" data0 "<<data[0]<<" data1 "<<data[1]<<" data2 "<<data[2]<<" data3 "<<data[3]<<" data4 "<<data[4]<<" data5 "<<data[5]<<" data6 "<<data[6]<<" data7 "<<data.size()<<endl;
	//Query id for the Query.
	int query = atoi(data[0].c_str());
	std::stringstream inttostring;
	inttostring << query;
	string queryid = inttostring.str();
	
	if(!qrel->existInQrel(queryid))
		continue;
	string docId = data[2];
	int rank = atoi(data[4].c_str());
//	cout<<"Rank"<<rank<<data[3]<<endl;
	/** If Queryid have changed save stats to the final vectors
	 * which store statistics of all read queries*/
	if(previous.compare(queryid) != 0) {
		if(effQueryCounter != 0) {
			vecNumberofRetrieved.push_back(numberofRetrievedCounter);
			vecNumberofRelevantRetrieved.push_back(numberofRelevantRetrievedCounter);
		listofRetrieved.push_back(*retrieved);
		listofRelevantRetrieved.push_back(*relevantRetrieved);
		numberofRetrievedCounter = 0;
		numberofRelevantRetrievedCounter = 0;
		retrieved = new vector<Record>();
		relevantRetrieved = new vector<Record>();
		}
	effQueryCounter++;
	vecQueryNo.push_back(queryid);
	vecNumberofRelevant.push_back(qrel->getNumberofRelevant(queryid));
	}		
	previous = queryid;
	numberofRetrievedCounter++;
	totalNumberofRetrieved++;
	Record *currrec = new Record(queryid,docId,rank);
	retrieved->push_back(*currrec);	
	//adding relevant document to the relevant retreived set
	if (qrel->isRelevantDoc(docId,queryid)) {
		relevantRetrieved->push_back(*currrec);
	numberofRelevantRetrievedCounter++;
	}

	} //end of data checking if condition	
}
	vecNumberofRetrieved.push_back(numberofRetrievedCounter);
	vecNumberofRelevantRetrieved.push_back(numberofRelevantRetrievedCounter);
	listofRetrieved.push_back(*retrieved);
	listofRelevantRetrieved.push_back(*relevantRetrieved);
	resultfile.close();

	vector<vector<Record> >::iterator it ;
	vector<Record>::iterator recorditr;
	for (it = listofRelevantRetrieved.begin();it != listofRelevantRetrieved.end();it++) {
		vector<Record> recordvec = *it;
		for (recorditr = recordvec.begin();recorditr != recordvec.end();recorditr++) {
			Record rec = *recorditr;
		cout<<"Query ID\t"<<rec.getQueryNo()<<"DOCID\t"<<rec.getDocNo()<<"Rank \t"<<rec.getRank()<<endl;
		}
	}	
	vector<int>::iterator noretrieved;
	vector<int>::iterator norelevantretrieved;
	for(noretrieved = vecNumberofRetrieved.begin(),norelevantretrieved = vecNumberofRelevantRetrieved.begin();noretrieved != vecNumberofRetrieved.end()&&norelevantretrieved != vecNumberofRelevantRetrieved.end();noretrieved++,norelevantretrieved++) {
	int retrievedrecords = *noretrieved;
	int relevantretrievedrecords = *norelevantretrieved;
	cout<<"Retrieved :\t"<<retrievedrecords<<" Relevant Retrieved:\t"<<relevantretrievedrecords<<endl;
	if (retrievedrecords != 0) {
	meanAveragePrecision += double((double)relevantretrievedrecords/retrievedrecords);
	//cout<<"MAP"<<double((double)relevantretrievedrecords/(double)retrievedrecords)<<endl;
	}
}
meanAveragePrecision = meanAveragePrecision/vecNumberofRelevant.size();
}


void
AdhocEvaluation::writeEvaluationResult() {
cout<<"Result for the run:\t"<<configobj.get_runname()<<endl;
cout<<"Mean Average Precision :\t"<<meanAveragePrecision;
}

int main(int argc,char **argv) {
CONFIG_TREC config;
config.setup_config(string(argv[1]));
AdhocEvaluation *eval = new AdhocEvaluation(config);
eval->evaluate();
eval->writeEvaluationResult();
}
