/*
 * Program Name: EECS 348 Assignment 7
 * Brief Description: MySQL database query program that performs 12 SQL queries 
 *                    on a university database containing student, faculty, course,
 *                    offering, and enrollment information. This program demonstrates
 *                    data-structured centered design and relational database coding.
 * 
 * Inputs: MySQL database connection credentials, database tables created from 
 *         Assignment7SQLTables.zip files (Course7, Faculty7, Student7, Offering7, Enrollment7)
 * 
 * Outputs: Formatted query results displayed to console, including student information,
 *          faculty details, enrollment data, and modified records after INSERT/UPDATE operations
 * 
 * Collaborators: None
 * Other Sources: ChatGPT, MySQL Connector/C++ documentation, Stack Overflow
 * Author: [Your Full Name]
 * Creation Date: November 20, 2025
 */

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Function prototypes for executing each query
void executeQuery1(sql::Connection* con);
void executeQuery2(sql::Connection* con);
void executeQuery3(sql::Connection* con);
void executeQuery4(sql::Connection* con);
void executeQuery5(sql::Connection* con);
void executeQuery6(sql::Connection* con);
void executeQuery7(sql::Connection* con);
void executeQuery8(sql::Connection* con);
void executeQuery9(sql::Connection* con);
void executeQuery10(sql::Connection* con);
void executeQuery11(sql::Connection* con);
void executeQuery12(sql::Connection* con);

// Helper function to print section headers
void printQueryHeader(int queryNum, const string& description) {
    cout << "\n" << string(80, '=') << "\n";
    cout << "Query " << queryNum << ": " << description << "\n";
    cout << string(80, '=') << "\n";
}

int main() {
    try {
        // Create MySQL driver instance
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        
        // Establish connection to MySQL server using provided credentials
        // Replace password with your real MySQL password
        sql::Connection* con = driver->connect(
            "tcp://mysql.eecs.ku.edu:3306",
            "348f25_h273m190",
            "DJswamp@123"
        );
        
        // Select the specific database to query
        con->setSchema("348f25_h273m190");
        
        // Execute each of the 12 required queries sequentially
        executeQuery1(con);
        executeQuery2(con);
        executeQuery3(con);
        executeQuery4(con);
        executeQuery5(con);
        executeQuery6(con);
        executeQuery7(con);
        executeQuery8(con);
        executeQuery9(con);
        executeQuery10(con);
        executeQuery11(con);
        executeQuery12(con);
        
        // Clean up database connection resources
        delete con;
        
        cout << "\n" << string(80, '=') << "\n";
        cout << "All queries completed successfully!\n";
        cout << string(80, '=') << "\n";
        
        return 0;
    }
    catch (sql::SQLException& e) {
        // Catch and display SQL connection or execution errors
        cerr << "SQLException: " << e.what() << "\n";
        cerr << "Error Code: " << e.getErrorCode() << "\n";
        return 1;
    }
}

// Query 1: Retrieve all student information for those whose major is 'IS'
void executeQuery1(sql::Connection* con) {
    printQueryHeader(1, "Retrieve all student information for those whose major is 'IS'");
    
    try {
        // Create statement object to execute SQL query
        sql::Statement* stmt = con->createStatement();
        
        // Execute SELECT query filtering by IS major
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7 WHERE StdMajor = 'IS'");
        
        // Print column headers for result set
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Major" << setw(8) << "Class" << setw(6) << "GPA" 
             << "Zip Code\n";
        cout << string(95, '-') << "\n";
        
        // Iterate through result set and display each row
        while (res->next()) {
            cout << left << setw(12) << res->getString("StdNo")
                 << setw(15) << res->getString("StdFirstName")
                 << setw(15) << res->getString("StdLastName")
                 << setw(15) << res->getString("StdCity")
                 << setw(6) << res->getString("StdState")
                 << setw(12) << res->getString("StdMajor")
                 << setw(8) << res->getString("StdClass")
                 << setw(6) << res->getDouble("StdGPA")
                 << res->getString("StdZip") << "\n";
        }
        
        // Clean up result set and statement resources
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 1: " << e.what() << "\n";
    }
}

// Query 2: Retrieve student numbers of students enrolled in more than two courses
void executeQuery2(sql::Connection* con) {
    printQueryHeader(2, "Retrieve the student numbers of students who have enrolled in more than two courses");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query using GROUP BY to count enrollments per student, HAVING to filter
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT StdNo, COUNT(*) as EnrollmentCount FROM Enrollment7 "
            "GROUP BY StdNo HAVING COUNT(*) > 2"
        );
        
        // Print column headers
        cout << left << setw(15) << "Student No" << "Enrollment Count\n";
        cout << string(30, '-') << "\n";
        
        // Display each student with more than 2 enrollments
        while (res->next()) {
            cout << left << setw(15) << res->getString("StdNo")
                 << res->getInt("EnrollmentCount") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 2: " << e.what() << "\n";
    }
}

// Query 3: List first and last names of professors employed for more than 15 years as of 2025
void executeQuery3(sql::Connection* con) {
    printQueryHeader(3, "List the first and last names of all professors who have been employed for more than 15 years as of 2025");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Employees hired in 2010 or earlier have 15+ years of service as of 2025
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT FacFirstName, FacLastName FROM Faculty7 "
            "WHERE YEAR(FacHireDate) <= 2010"
        );
        
        // Print column headers
        cout << left << setw(20) << "First Name" << "Last Name\n";
        cout << string(40, '-') << "\n";
        
        // Display each professor meeting the criteria
        while (res->next()) {
            cout << left << setw(20) << res->getString("FacFirstName")
                 << res->getString("FacLastName") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 3: " << e.what() << "\n";
    }
}

// Query 4: Retrieve all course numbers offered in Summer 2020
void executeQuery4(sql::Connection* con) {
    printQueryHeader(4, "Retrieve all course numbers from the Offering table that were offered in Summer 2020");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query filtering by SUMMER term and 2020 year
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT DISTINCT CourseNo FROM Offering7 "
            "WHERE OffTerm = 'SUMMER' AND OffYear = 2020"
        );
        
        // Print column header
        cout << "Course Number\n";
        cout << string(15, '-') << "\n";
        
        // Display each course offered in Summer 2020
        while (res->next()) {
            cout << res->getString("CourseNo") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 4: " << e.what() << "\n";
    }
}

// Query 5: List first and last names of professors living in ZIP code 98114
void executeQuery5(sql::Connection* con) {
    printQueryHeader(5, "List the first and last names of all professors from the Faculty table who live in ZIP code 98114");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query filtering by specific ZIP code (98114)
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT FacFirstName, FacLastName FROM Faculty7 WHERE FacZip = '98114'"
        );
        
        // Print column headers
        cout << left << setw(20) << "First Name" << "Last Name\n";
        cout << string(40, '-') << "\n";
        
        // Display professors in specified ZIP code
        while (res->next()) {
            cout << left << setw(20) << res->getString("FacFirstName")
                 << res->getString("FacLastName") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 5: " << e.what() << "\n";
    }
}

// Query 6: Retrieve the second-highest GPA from Student table
void executeQuery6(sql::Connection* con) {
    printQueryHeader(6, "Retrieve the second-highest GPA from the Student table");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query ordering by GPA descending and limiting to second row
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT DISTINCT StdGPA FROM Student7 ORDER BY StdGPA DESC LIMIT 1, 1"
        );
        
        // Print column header
        cout << "Second-Highest GPA\n";
        cout << string(20, '-') << "\n";
        
        // Display the second-highest GPA value
        if (res->next()) {
            cout << fixed << setprecision(2) << res->getDouble("StdGPA") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 6: " << e.what() << "\n";
    }
}

// Query 7: Retrieve first and last names of individuals in both Student and Faculty tables
void executeQuery7(sql::Connection* con) {
    printQueryHeader(7, "Retrieve the first and last names of individuals who appear in both the Student and Faculty tables");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query using inner join to find matching names
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT s.StdFirstName, s.StdLastName FROM Student7 s "
            "INNER JOIN Faculty7 f ON s.StdFirstName = f.FacFirstName "
            "AND s.StdLastName = f.FacLastName"
        );
        
        // Print column headers
        cout << left << setw(20) << "First Name" << "Last Name\n";
        cout << string(40, '-') << "\n";
        
        // Display individuals appearing in both tables
        while (res->next()) {
            cout << left << setw(20) << res->getString("StdFirstName")
                 << res->getString("StdLastName") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 7: " << e.what() << "\n";
    }
}

// Query 8: Retrieve student number, names, and course count for each student
void executeQuery8(sql::Connection* con) {
    printQueryHeader(8, "Retrieve the student number, first and last names, and the number of selected courses from the Student and Enrollment tables");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query using LEFT JOIN to include students with no enrollments
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT s.StdNo, s.StdFirstName, s.StdLastName, "
            "       COUNT(e.StdNo) AS CourseCount "
            "FROM Student7 s "
            "LEFT JOIN Enrollment7 e ON s.StdNo = e.StdNo "
            "GROUP BY s.StdNo, s.StdFirstName, s.StdLastName"
        );
        
        // Print column headers
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << "Course Count\n";
        cout << string(55, '-') << "\n";
        
        // Display each student with their course enrollment count
        while (res->next()) {
            cout << left << setw(12) << res->getString("StdNo")
                 << setw(15) << res->getString("StdFirstName")
                 << setw(15) << res->getString("StdLastName")
                 << res->getInt("CourseCount") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 8: " << e.what() << "\n";
    }
}

// Query 9: Retrieve names and salaries of top three highest-paid professors
void executeQuery9(sql::Connection* con) {
    printQueryHeader(9, "Retrieve the first and last names of professors, along with their salaries, for the top three highest-paid professors");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query ordering by salary descending and limiting to top 3
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT FacFirstName, FacLastName, FacSalary FROM Faculty7 "
            "ORDER BY FacSalary DESC LIMIT 3"
        );
        
        // Print column headers
        cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << "Salary\n";
        cout << string(60, '-') << "\n";
        
        // Display top 3 highest-paid faculty members
        while (res->next()) {
            cout << left << setw(20) << res->getString("FacFirstName")
                 << setw(20) << res->getString("FacLastName")
                 << fixed << setprecision(2) << res->getDouble("FacSalary") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 9: " << e.what() << "\n";
    }
}

// Query 10: Retrieve students with no enrollment records
void executeQuery10(sql::Connection* con) {
    printQueryHeader(10, "Retrieve all student information from the Student table for students who do not have any enrollment records");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute query using LEFT JOIN with NULL check to find students without enrollments
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT s.* FROM Student7 s LEFT JOIN Enrollment7 e ON s.StdNo = e.StdNo "
            "WHERE e.StdNo IS NULL"
        );
        
        // Print column headers
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Major" << setw(8) << "Class" << setw(6) << "GPA" 
             << "Zip Code\n";
        cout << string(95, '-') << "\n";
        
        // Display each student with no enrollment records
        while (res->next()) {
            cout << left << setw(12) << res->getString("StdNo")
                 << setw(15) << res->getString("StdFirstName")
                 << setw(15) << res->getString("StdLastName")
                 << setw(15) << res->getString("StdCity")
                 << setw(6) << res->getString("StdState")
                 << setw(12) << res->getString("StdMajor")
                 << setw(8) << res->getString("StdClass")
                 << setw(6) << res->getDouble("StdGPA")
                 << res->getString("StdZip") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 10: " << e.what() << "\n";
    }
}

// Query 11: Insert new student and display updated Student table
void executeQuery11(sql::Connection* con) {
    printQueryHeader(11, "Insert Alice Smith and display updated Student table");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute INSERT statement to add new junior CS student
        stmt->execute(
            "INSERT INTO Student7 (StdNo, StdFirstName, StdLastName, StdCity, StdState, "
            "StdZip, StdMajor, StdClass, StdGPA) "
            "VALUES ('888-88-8888', 'Alice', 'Smith', 'Topeka', 'KS', '66610', 'CS', 'JR', 3.85)"
        );
        
        cout << "Insert successful!\n\n";
        
        // Execute SELECT to display entire updated Student table
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7");
        
        // Print column headers
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Zip" << setw(8) << "Major" << setw(8) << "Class" 
             << "GPA\n";
        cout << string(95, '-') << "\n";
        
        // Display all rows including newly inserted record
        while (res->next()) {
            cout << left << setw(12) << res->getString("StdNo")
                 << setw(15) << res->getString("StdFirstName")
                 << setw(15) << res->getString("StdLastName")
                 << setw(15) << res->getString("StdCity")
                 << setw(6) << res->getString("StdState")
                 << setw(12) << res->getString("StdZip")
                 << setw(8) << res->getString("StdMajor")
                 << setw(8) << res->getString("StdClass")
                 << fixed << setprecision(2) << res->getDouble("StdGPA") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 11: " << e.what() << "\n";
    }
}

// Query 12: Update Bob Norbert's city and ZIP, then display updated Student table
void executeQuery12(sql::Connection* con) {
    printQueryHeader(12, "Update Bob Norbert's city and ZIP, then display updated Student table");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        // Execute UPDATE statement to change Bob Norbert's location
        stmt->execute(
            "UPDATE Student7 SET StdCity = 'Overland Park', StdZip = '66204' "
            "WHERE StdFirstName = 'Bob' AND StdLastName = 'Norbert'"
        );
        
        cout << "Update successful!\n\n";
        
        // Execute SELECT to display entire updated Student table
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7");
        
        // Print column headers
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Zip" << setw(8) << "Major" << setw(8) << "Class" 
             << "GPA\n";
        cout << string(95, '-') << "\n";
        
        // Display all rows with Bob Norbert's updated information
        while (res->next()) {
            cout << left << setw(12) << res->getString("StdNo")
                 << setw(15) << res->getString("StdFirstName")
                 << setw(15) << res->getString("StdLastName")
                 << setw(15) << res->getString("StdCity")
                 << setw(6) << res->getString("StdState")
                 << setw(12) << res->getString("StdZip")
                 << setw(8) << res->getString("StdMajor")
                 << setw(8) << res->getString("StdClass")
                 << fixed << setprecision(2) << res->getDouble("StdGPA") << "\n";
        }
        
        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error in Query 12: " << e.what() << "\n";
    }
}