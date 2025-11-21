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
 * Author: Hameem Mohammed
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

// Create/drop tables for Assignment 7
void createTables(sql::Connection* con);

// Helper function to print section headers
void printQueryHeader(int queryNum, const string& description) {
    cout << "\n" << string(80, '=') << "\n";
    cout << "Query " << queryNum << ": " << description << "\n";
    cout << string(80, '=') << "\n";
}

// Create the 5 tables with *7 suffix matching the provided .SQL schemas
void createTables(sql::Connection* con) {
    try {
        sql::Statement* stmt = con->createStatement();

        // Drop child tables first, then parents, to avoid foreign key dependency issues
        stmt->execute("DROP TABLE IF EXISTS Enrollment7");
        stmt->execute("DROP TABLE IF EXISTS Offering7");
        stmt->execute("DROP TABLE IF EXISTS Course7");
        stmt->execute("DROP TABLE IF EXISTS Faculty7");
        stmt->execute("DROP TABLE IF EXISTS Student7");

        // Student7 table (matches Student schema from student.SQL)
        stmt->execute(
            "CREATE TABLE Student7 ("
            " StdNo CHAR(11) PRIMARY KEY,"
            " StdFirstName VARCHAR(50),"
            " StdLastName VARCHAR(50),"
            " StdCity VARCHAR(50),"
            " StdState CHAR(2),"
            " StdZip CHAR(10),"
            " StdMajor CHAR(6),"
            " StdClass CHAR(6),"
            " StdGPA DECIMAL(3,2) DEFAULT 0"
            ")"
        );

        // Faculty7 table (matches Faculty schema from faculty.SQL)
        stmt->execute(
            "CREATE TABLE Faculty7 ("
            " FacNo CHAR(11) PRIMARY KEY,"
            " FacFirstName VARCHAR(30),"
            " FacLastName VARCHAR(30),"
            " FacCity VARCHAR(30),"
            " FacState CHAR(2),"
            " FacDept CHAR(10),"
            " FacRank CHAR(4),"
            " FacSalary DECIMAL(10,2),"
            " FacSupervisor CHAR(11),"
            " FacHireDate DATE,"
            " FacZipCode CHAR(10)"
            ")"
        );

        // Course7 table (matches Course schema from course.SQL)
        stmt->execute(
            "CREATE TABLE Course7 ("
            " CourseNo CHAR(6) PRIMARY KEY,"
            " CrsDesc VARCHAR(50),"
            " CrsUnits INTEGER"
            ")"
        );

        // Offering7 table (matches Offering schema from offering.SQL)
        stmt->execute(
            "CREATE TABLE Offering7 ("
            " OfferNo INTEGER PRIMARY KEY,"
            " CourseNo CHAR(6),"
            " OffTerm CHAR(6),"
            " OffYear INTEGER,"
            " OffLocation VARCHAR(30),"
            " OffTime VARCHAR(10),"
            " FacNo CHAR(11),"
            " OffDays CHAR(6) DEFAULT 'MW'"
            ")"
        );

        // Enrollment7 table (matches Enrollment schema from enrollment.SQL)
        stmt->execute(
            "CREATE TABLE Enrollment7 ("
            " OfferNo INTEGER,"
            " StdNo CHAR(11),"
            " EnrGrade DECIMAL(3,2),"
            " PRIMARY KEY (OfferNo, StdNo)"
            ")"
        );

        delete stmt;
    }
    catch (sql::SQLException& e) {
        cerr << "Error creating tables: " << e.what() << "\n";
        throw;
    }
}

int main() {
    try {
        // Create MySQL driver instance
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        
        // Establish connection to MySQL server using provided credentials
        // >>>>> CHANGE PASSWORD STRING BELOW TO YOUR REAL MYSQL PASSWORD <<<<<
        sql::Connection* con = driver->connect(
            "tcp://mysql.eecs.ku.edu:3306",
            "348f25_h273m190",
            "YOUR_MYSQL_PASSWORD_HERE"
        );
        
        // Select the specific database to query
        con->setSchema("348f25_h273m190");

        // Create the Assignment 7 tables (drop if they already exist)
        createTables(con);
        
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
        sql::Statement* stmt = con->createStatement();
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT * FROM Student7 WHERE StdMajor = 'IS'"
        );
        
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Major" << setw(8) << "Class" << setw(6) << "GPA" 
             << "Zip Code\n";
        cout << string(95, '-') << "\n";
        
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
        cerr << "Error in Query 1: " << e.what() << "\n";
    }
}

// Query 2: Retrieve student numbers of students enrolled in more than two courses
void executeQuery2(sql::Connection* con) {
    printQueryHeader(2, "Retrieve the student numbers of students who have enrolled in more than two courses");
    
    try {
        sql::Statement* stmt = con->createStatement();
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT StdNo, COUNT(*) AS EnrollmentCount FROM Enrollment7 "
            "GROUP BY StdNo HAVING COUNT(*) > 2"
        );
        
        cout << left << setw(15) << "Student No" << "Enrollment Count\n";
        cout << string(30, '-') << "\n";
        
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
        
        // Include those hired in 2010 or earlier: YEAR(FacHireDate) <= 2010
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT FacFirstName, FacLastName FROM Faculty7 "
            "WHERE YEAR(FacHireDate) <= 2010"
        );
        
        cout << left << setw(20) << "First Name" << "Last Name\n";
        cout << string(40, '-') << "\n";
        
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
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT DISTINCT CourseNo FROM Offering7 "
            "WHERE OffTerm = 'SUMMER' AND OffYear = 2020"
        );
        
        cout << "Course Number\n";
        cout << string(15, '-') << "\n";
        
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
        
        // Match any ZIP that starts with 98114 (e.g., 98114-1332)
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT FacFirstName, FacLastName FROM Faculty7 "
            "WHERE FacZipCode LIKE '98114%'"
        );
        
        cout << left << setw(20) << "First Name" << "Last Name\n";
        cout << string(40, '-') << "\n";
        
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
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT DISTINCT StdGPA FROM Student7 "
            "ORDER BY StdGPA DESC LIMIT 1, 1"
        );
        
        cout << "Second-Highest GPA\n";
        cout << string(20, '-') << "\n";
        
        if (res->next()) {
            cout << fixed << setprecision(2) << res->getDouble("StdGPA") << "\n";
        } else {
            cout << "Not enough distinct GPA values.\n";
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
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT s.StdFirstName, s.StdLastName FROM Student7 s "
            "INNER JOIN Faculty7 f ON s.StdFirstName = f.FacFirstName "
            "AND s.StdLastName = f.FacLastName"
        );
        
        cout << left << setw(20) << "First Name" << "Last Name\n";
        cout << string(40, '-') << "\n";
        
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
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT s.StdNo, s.StdFirstName, s.StdLastName, "
            "       COUNT(e.OfferNo) AS CourseCount "
            "FROM Student7 s "
            "LEFT JOIN Enrollment7 e ON s.StdNo = e.StdNo "
            "GROUP BY s.StdNo, s.StdFirstName, s.StdLastName"
        );
        
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << "Course Count\n";
        cout << string(55, '-') << "\n";
        
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
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT FacFirstName, FacLastName, FacSalary FROM Faculty7 "
            "ORDER BY FacSalary DESC LIMIT 3"
        );
        
        cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << "Salary\n";
        cout << string(60, '-') << "\n";
        
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
        
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT s.* FROM Student7 s LEFT JOIN Enrollment7 e ON s.StdNo = e.StdNo "
            "WHERE e.StdNo IS NULL"
        );
        
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Major" << setw(8) << "Class" << setw(6) << "GPA" 
             << "Zip Code\n";
        cout << string(95, '-') << "\n";
        
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
        
        // Insert junior CS student Alice Smith
        stmt->execute(
            "INSERT INTO Student7 (StdNo, StdFirstName, StdLastName, StdCity, StdState, "
            "StdZip, StdMajor, StdClass, StdGPA) "
            "VALUES ('888-88-8888', 'Alice', 'Smith', 'Topeka', 'KS', '66610', 'CS', 'JR', 3.85)"
        );
        
        cout << "Insert successful!\n\n";
        
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7");
        
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Zip" << setw(8) << "Major" << setw(8) << "Class" 
             << "GPA\n";
        cout << string(95, '-') << "\n";
        
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
        
        // Update BOB NORBERT to Overland Park, KS ZIP 66204
        stmt->execute(
            "UPDATE Student7 SET StdCity = 'Overland Park', StdZip = '66204' "
            "WHERE StdFirstName = 'BOB' AND StdLastName = 'NORBERT'"
        );
        
        cout << "Update successful!\n\n";
        
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7");
        
        cout << left << setw(12) << "Std No" << setw(15) << "First Name" 
             << setw(15) << "Last Name" << setw(15) << "City" << setw(6) << "State" 
             << setw(12) << "Zip" << setw(8) << "Major" << setw(8) << "Class" 
             << "GPA\n";
        cout << string(95, '-') << "\n";
        
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
