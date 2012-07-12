/* @file trec_qrel.h
 * @abstract Header file for TREC Relevance Judgement InMemory.
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

#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <config_file.h>
#include <set>

using namespace std;

class TrecQrel {

protected:
	FILE * fqrels;

public:
	int totalnoifrelevantdocs;
	
	TRECQREL(CONFIG_TREC & config) {
	string reljudgement = config.get_relfile().c_str();
	fqrels = fopen(reljudgement,"r");
	if(!fqrels) {
		cout<<"ERROR - can't open topic file" << config.get_relfile().c_str() << "for reading" << endl;
		std::exit(-1);
	}
	}
	
	/*  To get Query ids of all the queries present in relevance judgement pool.
 	 *  @return query ids of queries in pool 
	*/
	string[] getQueryIds();

    /* Get the relevant document ids for a Query  and of particular grade
     *	@param grade - Integer grade for which  relevant document need to be returned.
     *	@param queryid - Queryid of query for which the documents ned to be returned.
	 *  @return - set of the relevant document for query queryid and grade grade
     */

	set<string> getRelevantDocument(int grade,string queryid);
	
    /* Get the relevant document ids for a given Query 
     *	@param queryid - Queryid of query for which the documents ned to be returned.
	 *  @return - set of the relevant document for query queryid from all grades
     */
		
	set<string> getRelevantDocument(string queryid);

	/* Get number of relevant document for the query.
	 * @param queryid - Queryid of query for which number of relevant documents to be returned.
	 * @return - Number of relevant documents for the query.
	 */

	int getNumberofRelevant(string queryid);

	/* Get total number of queries in the pool.
	 * @return Number of  query in the pool.
	 */
	
	int getNumberofQueries();
	
	/**
	 * Check whether Query with given identifier exist in relvance assessment
	 * @param queryid - identifier of the Query.
	 * @return true if the query exist in assessment pool,false otherwise.
	 */
	
	bool existInQrel(string queryid);
	
	/**
	 * Checks if the document is relvant or not for the query given by the query identifier.
	 * @param queryid - Identifier for the query.
	 * @param docno - Identifier for the document to be checked for relevant or not.
	 * @return true if document is relevant for the given query.
	 */
	
	 bool isRelevantDoc(string docno,string queryid);

	/**
	 * Get grade of the document for the particular query.
	 * @param queryid - Identifier for the query.
	 * @param docno - Identifier for the document to be checked for grade.
	 * @return int grade of the document for query if available in pool,else default grade 0.
	 */

 	int getGrade(string docno,string queryid);
}
