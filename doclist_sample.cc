/**
 * Copyright 2008 Google Inc. All Rights Reserved.
 * Author: e.bidelman@google.com (Eric Bidelman)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "gdata/util/string_utils.h"
#include "gdata/client/doc_list_service.h"

using namespace std;
using gdata::client::DocListService;
using gdata::util::StringUtils;

enum {LIST_ALL = 1,
      LIST_DOCSFOLDERS,
      SEARCH_DOCS,
      UPLOAD_DOC,
      DELETE_DOC,
      EXIT};
const int NUM_MAIN_MENU_OPTIONS = 6;
const string MAIN_MENU[NUM_MAIN_MENU_OPTIONS] =
     {"List all of your documents",
      "List specific documents/folders",
      "Search your documents",
      "Upload a document",
      "Delete a document/folder",
      "Exit"};

const int NUM_LIST_DOC_OPTIONS = 9;
const string LIST_DOC_MENU[NUM_LIST_DOC_OPTIONS] =
    {"Folders",
     "Word documents",
     "Spreadsheets",
     "Presentations",
     "Starred documents",
     "Trashed documents\n",
     "List folder contents",
     "List sharing permissions\n",
     "< Back"};
enum {LIST_FOLDERS = 1,
      LIST_DOCUMENTS,
      LIST_SPREADSHEETS,
      LIST_PRESOS,
      LIST_STARRED,
      LIST_TRASHED,
      LIST_DOCS_IN_FOLDER,
      LIST_ACLS,
      BACK};

void PrintMainMenu();
void PrintListDocSubMenu();
int GetMenuChoice(int num_options);

int main(int argc, char *argv[]) {
  string email, password;
  
  if (argc != 3) {
    cout << "Email address: ";
    cin >> email;
    cout << "password: ";
    cin >> password;
  } else {
    email = argv[1];
    password = argv[2];
  }

  cout << setfill('=') << setw(60) << "=" << endl
       << "Documents List API Sample" << endl
       << setfill('=') << setw(60) << "=" << endl
       << "Authenticating " << email << "...";
  DocListService service(email, password, "DocListCppSample-v1");
  cout << "OK!" << endl;

  while (1) {
    string url, q, doc_title, doc_path;
    int index, sub_menu_choice;
    vector< map<string, string> > doc_listing;
    string doc_list_feed = DocListService::kDocListScope +
                           DocListService::kDocListFeed;
    PrintMainMenu();

    switch (GetMenuChoice(NUM_MAIN_MENU_OPTIONS)) {
      case LIST_ALL:
        url = doc_list_feed;
        cout << "Retrieving all documents: " << url << endl << endl;
        doc_listing = service.ListDocuments(url);
        break;

      case LIST_DOCSFOLDERS:
        do {
          PrintListDocSubMenu();
          sub_menu_choice = GetMenuChoice(NUM_LIST_DOC_OPTIONS);

          switch (sub_menu_choice) {
            case LIST_FOLDERS:
              url = doc_list_feed + DocListService::kFolderCategory +
                  "?showfolders=true";
              cout << "Retrieving list of folders: " << url << endl << endl;
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_DOCUMENTS:
              url = doc_list_feed + DocListService::kDocumentCategory;
              cout << "Retrieving word docuemnts: " << url << endl << endl;
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_SPREADSHEETS:
              url = doc_list_feed + DocListService::kSpreadsheetCategory;
              cout << "Retrieving spreadsheets: " << url << endl << endl;
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_PRESOS:
              url = doc_list_feed + DocListService::kPresentationCategory;
              cout << "Retrieving presentations: " << url << endl << endl;
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_STARRED:
              url = doc_list_feed + DocListService::kStarredCategory;
              cout << "Retrieving starred documents: " << url << endl << endl;
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_TRASHED:
              url = doc_list_feed + DocListService::kTrashedCategory;
              cout << "Retrieving list of trashed docs: " << url << "\n\n";
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_DOCS_IN_FOLDER:
              url = doc_list_feed + DocListService::kFolderCategory +
                    "?showfolders=true";

              doc_listing = service.ListDocuments(url, false);
              for (unsigned int row = 0; row < doc_listing.size(); ++row) {
                cout << row + 1 << ". [" << doc_listing[row]["doc_type"]
                     << "] " << doc_listing[row]["title"] << endl;
              }
              cout << "\nWhich folder? ";
              index = GetMenuChoice(doc_listing.size()) - 1;
              url = doc_listing[index]["content_src"];
              cout << "Retrieving contents of folder '"
                   << doc_listing[index]["title"] << "': " << url << "\n\n";
              doc_listing = service.ListDocuments(url);
              break;

            case LIST_ACLS:
              url = doc_list_feed;
              doc_listing = service.ListDocuments(url, false);
              for (unsigned int row = 0; row < doc_listing.size(); ++row) {
                cout << row + 1 << ". [" << doc_listing[row]["doc_type"]
                     << "] " << doc_listing[row]["title"] << endl;
              }
              cout << "\nWhich document? ";
              index = GetMenuChoice(doc_listing.size()) - 1;
              url = doc_listing[index]["acl_feedLink"];
              cout << "Retrieving ACLs for document '"
                   <<  doc_listing[index]["title"] << "': " << url << "\n\n";
              service.ListAcls(url);
              break;

            case BACK:
              // pass through
              break;

            default:
              cerr << "Error - unknown sub-menu selection\n";
              exit(1);
          }
        } while (sub_menu_choice != BACK);
        break;

      case SEARCH_DOCS:
        url = doc_list_feed;
        cout << "Enter full text query: ";
        cin.ignore();
        getline(cin, q);
        url += "?q=" + StringUtils::find_and_replace(q, " ", "%20");
        cout << "Searching documents for '" << q << "': " << url << "\n\n";

        service.ListDocuments(url);
        break;

      case UPLOAD_DOC:
        cout << "File name to upload: ";
        cin >> doc_path;
        cout << "Enter a document title: ";
        cin.ignore();
        getline(cin, doc_title);

        service.UploadDoc(doc_path, doc_title);
        break;

      case DELETE_DOC:
        url = doc_list_feed + "?showfolders=true";
        doc_listing = service.ListDocuments(url, false);
        for (unsigned int row = 0; row < doc_listing.size(); ++row) {
          cout << row + 1 << ". [" << doc_listing[row]["doc_type"]
               << "] "<< doc_listing[row]["title"] << endl;
        }
        cout << "\nDelete which document? ";
        index = GetMenuChoice(doc_listing.size()) - 1;

        service.DeleteDoc(doc_listing[index]["edit_link"],
                          doc_listing[index]["etag"]);
        break;

      case EXIT:
        cout << "\nGoodbye :)\n\n";
        exit(0);
        break;

      default:
        cerr << "Error - unknown menu selection";
        exit(1);
    }
  }
  return 0;
}

void PrintMainMenu() {
  cout << endl << "Main Menu" << endl
       << setfill('-') << setw(40) << "-" << endl;
  for (int i = 0; i < NUM_MAIN_MENU_OPTIONS; ++i) {
    cout << i + 1 << ". " << MAIN_MENU[i] << endl;
  }
  cout << endl;
}

void PrintListDocSubMenu() {
  string title = "List documents sub-menu";

  cout << endl << title << endl
       << setfill('-') << setw(title.length()) << "-" << endl;
  for (int i = 0; i < NUM_LIST_DOC_OPTIONS; ++i) {
    cout << i + 1 << ". " << LIST_DOC_MENU[i] << endl;
  }
  cout << endl;
}

int GetMenuChoice(int num_options) {
  bool good_choice = false;
  int choice;

  do {
    cout << "> ";
    cin >> choice;

    if (!cin || (choice < 1 || choice > num_options)) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Invalid selection.  Please select 1-" << num_options << endl;
    } else {
      good_choice = true;
    }
  } while (!good_choice);

  return choice;
}
