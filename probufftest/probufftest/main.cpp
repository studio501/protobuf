//
//  main.cpp
//  probufftest
//
//  Created by 汤文 on 2020/8/8.
//  Copyright © 2020 汤文. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

#include "Common.pb.h"
#include "person.pb.h"

using namespace std;

void add_oneRequest(cokprobuf::Request * request){
    request->set_content("this is contents");
    cokprobuf::Header *header = request->mutable_header();
    //cokprobuf::Header header;
    header->set_commandid(123);
    //request->set_allocated_header(&header);
}

void print_oneRequest(cokprobuf::Request * request){
    cout<<"content is: "<<request->content()<<endl;
    const cokprobuf::Header * header = &request->header();
    cout<<"header is: "<<header->commandid()<<endl;
}

bool wirte_test(const char * file_name){
    
    cokprobuf::Request request;
    
    {
        //read
        fstream input(file_name, ios::in | ios::binary);
        if(!input){
            cout<<"file not found"<<endl;
            
        }else if (!request.ParseFromIstream(&input)){
            cout<<"failed to parse address book.";
            return -1;
        }
    }
    
    // add one request
    add_oneRequest(&request);
    
    string res1;
    request.SerializeToString(&res1);
    
    {
        //write
        fstream output(file_name,ios::out | ios::trunc | ios::binary);
        if(!request.SerializeToOstream(&output)){
            cerr<< "failed to write "<<endl;
            return -1;
        }
    }
    
    return 0;
}

bool read_test(const char * file_name){
    
    
    cokprobuf::Request request;
    {
        //red
        fstream input(file_name,ios::in| ios::binary);
        if(!request.ParseFromIstream(&input)){
            cerr<<"failed to parse"<<endl;
            return -1;
        }
    }
    
    print_oneRequest(&request);
    
    
    return 0;
    
}

// This function fills in a Person message based on user input.
void PromptForAddress(cokprobuf::Person* person) {
    
    person->set_id(1);
    person->set_name("tangwen");
    person->set_email("gg@163.com");
    
    cokprobuf::Person::PhoneNumber* phone_number = person->add_phones();
    phone_number->set_number("18210436456");
    phone_number->set_type(cokprobuf::Person::MOBILE);
}


// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const cokprobuf::AddressBook& address_book) {
  for (int i = 0; i < address_book.people_size(); i++) {
    const cokprobuf::Person& person = address_book.people(i);

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;

    for (int j = 0; j < person.phones_size(); j++) {
      const cokprobuf::Person::PhoneNumber& phone_number = person.phones(j);

      switch (phone_number.type()) {
        case cokprobuf::Person::MOBILE:
          cout << "  Mobile phone #: ";
          break;
        case cokprobuf::Person::HOME:
          cout << "  Home phone #: ";
          break;
        case cokprobuf::Person::WORK:
          cout << "  Work phone #: ";
          break;
      }
      cout << phone_number.number() << endl;
    }
  }
}


bool write_person(const char * file_name){
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
   


    cokprobuf::AddressBook address_book;

    {
      // Read the existing address book.
      fstream input(file_name, ios::in | ios::binary);
      if (!input) {
        cout << file_name << ": File not found.  Creating a new file." << endl;
      } else if (!address_book.ParseFromIstream(&input)) {
        cerr << "Failed to parse address book." << endl;
        return -1;
      }
    }

    // Add an address.
    PromptForAddress(address_book.add_people());

    {
      // Write the new address book back to disk.
      fstream output(file_name, ios::out | ios::trunc | ios::binary);
      if (!address_book.SerializeToOstream(&output)) {
        cerr << "Failed to write address book." << endl;
        return -1;
      }
    }


    return 0;
}

bool read_person(const char * file_name){
    cokprobuf::AddressBook address_book;

    {
      // Read the existing address book.
      fstream input(file_name, ios::in | ios::binary);
      if (!address_book.ParseFromIstream(&input)) {
        cerr << "Failed to parse address book." << endl;
        return -1;
      }
    }

    ListPeople(address_book);

    return 0;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    const char * file_name ="/Users/tangwen/Documents/my_projects/protobufTest/CommonMsg";
    const char * file_name2 ="/Users/tangwen/Documents/my_projects/protobufTest/AddressBook";
    
    
    wirte_test(file_name);
    //read_test(file_name);
//    for(int i=0;i<2000;i++){
//        write_person(file_name2);
//    }
    
    read_person(file_name2);
    
    std::cout << "Hello, World!\n";
    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
