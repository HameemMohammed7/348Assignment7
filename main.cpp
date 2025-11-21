/*
 * Program Name: EECS 348 Assignment 7
 * Brief Description:
 *   Connects to the EECS MySQL server, clones the base tables into
 *   Course7, Faculty7, Student7, Offering7, and Enrollment7, then runs
 *   the 12 assignment queries and prints the results in a readable format.
 *
 * Inputs:
 *   - MySQL credentials for the 348f25_* database on mysql.eecs.ku.edu
 *
 * Outputs:
 *   - Formatted query results printed to standard output
 *
 * Collaborators: None (logic and formatting written independently)
 * Other Sources: MySQL Connector/C++ documentation
 * Author: Hameem
 * Creation Date: November 20, 2025
 */

#include <iostream>
#include <iomanip>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using std::cout;
using std::cerr;
using std::endl;
using std::left;
using std::setw;
using std::string;

// Forward declarations
void cloneTablesTo7(sql::Connection* con);

void runQuery1(sql::Connection* con);
void runQuery2(sql::Connection* con);
void runQuery3(sql::Connection* con);
void runQuery4(sql::Connection* con);
void runQuery5(sql::Connection* con);
void runQuery6(sql::Connection* con);
void runQuery7(sql::Connection* con);
void runQuery8(sql::Connection* con);
void runQuery9(sql::Connection* con);
void runQuery10(sql::Connection* con);
void runQuery11(sql::Connection* con);
void runQuery12(sql::Connection* con);

static void printSeparator(int width = 90) {
    cout << string(width, '=') << "\n";
}

static void printQueryTitle(int num, const string& text) {
    printSeparator();
    cout << "Query " << num << " - " << text << "\n";
    printSeparator();
    cout << "\n";
}

int main() {
    try {
        // Get driver
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

        // TODO: put YOUR OWN credentials here, not your friend's
        // Example:
        //   username: 348f25_h273m190
        //   password: (the one from Liangqin Ren in Canvas for Lab 12)
        sql::Connection* con = driver->connect(
            "tcp://mysql.eecs.ku.edu:3306",
            "348f25_h273m190",   // <-- CHANGE THIS TO YOUR MySQL USERNAME
            "eu7Eejai" // <-- CHANGE THIS TO YOUR MySQL PASSWORD
        );

        // Use your database schema (same as your username)
        con->setSchema("348f25_h273m190"); // <-- CHANGE THIS TO MATCH THE USER ABOVE

        cout << "Connected to MySQL.\n\n";

        // Make sure *7 tables exist and are populated from the base tables
        cloneTablesTo7(con);

        // Run all 12 queries in order
        runQuery1(con);
        runQuery2(con);
        runQuery3(con);
        runQuery4(con);
        runQuery5(con);
        runQuery6(con);
        runQuery7(con);
        runQuery8(con);
        runQuery9(con);
        runQuery10(con);
        runQuery11(con);
        runQuery12(con);

        cout << "\n";
        printSeparator();
        cout << "All queries finished.\n";
        printSeparator();

        delete con;
        return 0;
    }
    catch (sql::SQLException& e) {
        cerr << "SQL Exception: " << e.what() << "\n";
        cerr << "Error code: " << e.getErrorCode() << "\n";
        return 1;
    }
}

// ---------------------------------------------------------------------------
// Create/drop/clone tables
// ---------------------------------------------------------------------------

void cloneTablesTo7(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();

    // Drop 7-suffixed tables if they already exist
    stmt->execute("DROP TABLE IF EXISTS Enrollment7");
    stmt->execute("DROP TABLE IF EXISTS Offering7");
    stmt->execute("DROP TABLE IF EXISTS Course7");
    stmt->execute("DROP TABLE IF EXISTS Faculty7");
    stmt->execute("DROP TABLE IF EXISTS Student7");

    // Clone structure & data from base tables: Course, Faculty, Offering, Student, Enrollment
    stmt->execute("CREATE TABLE Course7 LIKE Course");
    stmt->execute("INSERT INTO Course7 SELECT * FROM Course");

    stmt->execute("CREATE TABLE Faculty7 LIKE Faculty");
    stmt->execute("INSERT INTO Faculty7 SELECT * FROM Faculty");

    stmt->execute("CREATE TABLE Offering7 LIKE Offering");
    stmt->execute("INSERT INTO Offering7 SELECT * FROM Offering");

    stmt->execute("CREATE TABLE Student7 LIKE Student");
    stmt->execute("INSERT INTO Student7 SELECT * FROM Student");

    stmt->execute("CREATE TABLE Enrollment7 LIKE Enrollment");
    stmt->execute("INSERT INTO Enrollment7 SELECT * FROM Enrollment");

    delete stmt;

    cout << "Cloned base tables into *7 tables.\n\n";
}

// ---------------------------------------------------------------------------
// Query 1
// ---------------------------------------------------------------------------

void runQuery1(sql::Connection* con) {
    printQueryTitle(1, "All students whose major is 'IS' (from Student7)");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT * FROM Student7 WHERE StdMajor = 'IS'"
    );

    cout << left
         << setw(15) << "StdNo"
         << setw(15) << "FirstName"
         << setw(15) << "LastName"
         << setw(15) << "City"
         << setw(6)  << "State"
         << setw(8)  << "Major"
         << setw(8)  << "Class"
         << setw(6)  << "GPA"
         << "Zip\n";

    cout << string(95, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(15) << res->getString("StdNo")
             << setw(15) << res->getString("StdFirstName")
             << setw(15) << res->getString("StdLastName")
             << setw(15) << res->getString("StdCity")
             << setw(6)  << res->getString("StdState")
             << setw(8)  << res->getString("StdMajor")
             << setw(8)  << res->getString("StdClass")
             << setw(6)  << res->getDouble("StdGPA")
             << res->getString("StdZip")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 2
// ---------------------------------------------------------------------------

void runQuery2(sql::Connection* con) {
    printQueryTitle(2, "Student numbers of students enrolled in more than two courses");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT StdNo, COUNT(*) AS NumCourses "
        "FROM Enrollment7 "
        "GROUP BY StdNo "
        "HAVING COUNT(*) > 2"
    );

    cout << left << setw(15) << "StdNo"
         << setw(12) << "NumCourses"
         << "\n";
    cout << string(27, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(15) << res->getString("StdNo")
             << setw(12) << res->getInt("NumCourses")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 3
// ---------------------------------------------------------------------------

void runQuery3(sql::Connection* con) {
    printQueryTitle(3, "Professors employed more than 15 years as of 2025 (hire year <= 2010)");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT FacFirstName, FacLastName "
        "FROM Faculty7 "
        "WHERE YEAR(FacHireDate) <= 2010"
    );

    cout << left
         << setw(20) << "FirstName"
         << setw(20) << "LastName"
         << "\n";
    cout << string(40, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(20) << res->getString("FacFirstName")
             << setw(20) << res->getString("FacLastName")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 4
// ---------------------------------------------------------------------------

void runQuery4(sql::Connection* con) {
    printQueryTitle(4, "Course numbers offered in SUMMER 2020");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT DISTINCT CourseNo "
        "FROM Offering7 "
        "WHERE OffTerm = 'SUMMER' AND OffYear = 2020"
    );

    cout << left << setw(12) << "CourseNo" << "\n";
    cout << string(12, '-') << "\n";

    while (res->next()) {
        cout << left << setw(12) << res->getString("CourseNo") << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 5
// ---------------------------------------------------------------------------

void runQuery5(sql::Connection* con) {
    printQueryTitle(5, "Professors who live in ZIP code 98114 (Faculty7)");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT FacFirstName, FacLastName "
        "FROM Faculty7 "
        "WHERE FacZipCode LIKE '98114%'"
    );

    cout << left
         << setw(20) << "FirstName"
         << setw(20) << "LastName"
         << "\n";
    cout << string(40, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(20) << res->getString("FacFirstName")
             << setw(20) << res->getString("FacLastName")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 6
// ---------------------------------------------------------------------------

void runQuery6(sql::Connection* con) {
    printQueryTitle(6, "Second-highest GPA in Student7");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT DISTINCT StdGPA "
        "FROM Student7 "
        "ORDER BY StdGPA DESC "
        "LIMIT 1, 1"
    );

    cout << left << setw(10) << "StdGPA" << "\n";
    cout << string(10, '-') << "\n";

    if (res->next()) {
        cout << left << setw(10) << res->getDouble("StdGPA") << "\n";
    } else {
        cout << "Not enough distinct GPA values.\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 7
// ---------------------------------------------------------------------------

void runQuery7(sql::Connection* con) {
    printQueryTitle(7, "People who appear in both Student7 and Faculty7 (by first+last name)");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT s.StdFirstName AS FirstName, s.StdLastName AS LastName "
        "FROM Student7 s "
        "INNER JOIN Faculty7 f "
        "  ON s.StdFirstName = f.FacFirstName "
        " AND s.StdLastName  = f.FacLastName"
    );

    cout << left
         << setw(20) << "FirstName"
         << setw(20) << "LastName"
         << "\n";
    cout << string(40, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(20) << res->getString("FirstName")
             << setw(20) << res->getString("LastName")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 8
// ---------------------------------------------------------------------------

void runQuery8(sql::Connection* con) {
    printQueryTitle(8, "Student number, name, and count of selected courses");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT s.StdNo, s.StdFirstName, s.StdLastName, "
        "       COUNT(e.OfferNo) AS CourseCount "
        "FROM Student7 s "
        "LEFT JOIN Enrollment7 e ON s.StdNo = e.StdNo "
        "GROUP BY s.StdNo, s.StdFirstName, s.StdLastName"
    );

    cout << left
         << setw(15) << "StdNo"
         << setw(15) << "FirstName"
         << setw(15) << "LastName"
         << setw(12) << "CourseCount"
         << "\n";
    cout << string(57, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(15) << res->getString("StdNo")
             << setw(15) << res->getString("StdFirstName")
             << setw(15) << res->getString("StdLastName")
             << setw(12) << res->getInt("CourseCount")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 9
// ---------------------------------------------------------------------------

void runQuery9(sql::Connection* con) {
    printQueryTitle(9, "Top 3 highest-paid professors (Faculty7)");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT FacFirstName, FacLastName, FacSalary "
        "FROM Faculty7 "
        "ORDER BY FacSalary DESC "
        "LIMIT 3"
    );

    cout << left
         << setw(20) << "FirstName"
         << setw(20) << "LastName"
         << setw(12) << "Salary"
         << "\n";
    cout << string(52, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(20) << res->getString("FacFirstName")
             << setw(20) << res->getString("FacLastName")
             << setw(12) << res->getDouble("FacSalary")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 10
// ---------------------------------------------------------------------------

void runQuery10(sql::Connection* con) {
    printQueryTitle(10, "Students in Student7 with NO enrollment records in Enrollment7");

    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery(
        "SELECT s.* "
        "FROM Student7 s "
        "LEFT JOIN Enrollment7 e ON s.StdNo = e.StdNo "
        "WHERE e.StdNo IS NULL"
    );

    cout << left
         << setw(15) << "StdNo"
         << setw(15) << "FirstName"
         << setw(15) << "LastName"
         << setw(15) << "City"
         << setw(6)  << "State"
         << setw(8)  << "Major"
         << setw(8)  << "Class"
         << setw(6)  << "GPA"
         << "Zip\n";
    cout << string(95, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(15) << res->getString("StdNo")
             << setw(15) << res->getString("StdFirstName")
             << setw(15) << res->getString("StdLastName")
             << setw(15) << res->getString("StdCity")
             << setw(6)  << res->getString("StdState")
             << setw(8)  << res->getString("StdMajor")
             << setw(8)  << res->getString("StdClass")
             << setw(6)  << res->getDouble("StdGPA")
             << res->getString("StdZip")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 11
// ---------------------------------------------------------------------------

void runQuery11(sql::Connection* con) {
    printQueryTitle(11, "Insert Alice Smith as a junior CS student and show updated Student7");

    sql::Statement* stmt = con->createStatement();

    // Insert Alice only if she doesn't already exist (defensive)
    stmt->execute(
        "INSERT INTO Student7 (StdNo, StdFirstName, StdLastName, StdCity, StdState, "
        " StdZip, StdMajor, StdClass, StdGPA) "
        "VALUES ('888-88-8888', 'ALICE', 'SMITH', 'TOPEKA', 'KS', '66610', 'CS', 'JR', 3.85)"
    );

    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7");

    cout << left
         << setw(15) << "StdNo"
         << setw(15) << "FirstName"
         << setw(15) << "LastName"
         << setw(15) << "City"
         << setw(6)  << "State"
         << setw(12) << "Zip"
         << setw(8)  << "Major"
         << setw(8)  << "Class"
         << setw(6)  << "GPA"
         << "\n";
    cout << string(100, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(15) << res->getString("StdNo")
             << setw(15) << res->getString("StdFirstName")
             << setw(15) << res->getString("StdLastName")
             << setw(15) << res->getString("StdCity")
             << setw(6)  << res->getString("StdState")
             << setw(12) << res->getString("StdZip")
             << setw(8)  << res->getString("StdMajor")
             << setw(8)  << res->getString("StdClass")
             << setw(6)  << res->getDouble("StdGPA")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}

// ---------------------------------------------------------------------------
// Query 12
// ---------------------------------------------------------------------------

void runQuery12(sql::Connection* con) {
    printQueryTitle(12, "Update Bob Norbert to Overland Park, KS and show updated Student7");

    sql::Statement* stmt = con->createStatement();

    stmt->execute(
        "UPDATE Student7 "
        "SET StdCity = 'OVERLAND PARK', "
        "    StdState = 'KS', "
        "    StdZip   = '66204' "
        "WHERE StdFirstName = 'BOB' AND StdLastName = 'NORBERT'"
    );

    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM Student7");

    cout << left
         << setw(15) << "StdNo"
         << setw(15) << "FirstName"
         << setw(15) << "LastName"
         << setw(15) << "City"
         << setw(6)  << "State"
         << setw(12) << "Zip"
         << setw(8)  << "Major"
         << setw(8)  << "Class"
         << setw(6)  << "GPA"
         << "\n";
    cout << string(100, '-') << "\n";

    while (res->next()) {
        cout << left
             << setw(15) << res->getString("StdNo")
             << setw(15) << res->getString("StdFirstName")
             << setw(15) << res->getString("StdLastName")
             << setw(15) << res->getString("StdCity")
             << setw(6)  << res->getString("StdState")
             << setw(12) << res->getString("StdZip")
             << setw(8)  << res->getString("StdMajor")
             << setw(8)  << res->getString("StdClass")
             << setw(6)  << res->getDouble("StdGPA")
             << "\n";
    }

    cout << "\n";

    delete res;
    delete stmt;
}
