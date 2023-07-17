#include <iostream>
#include <fstream>
using namespace std;

class User;
class Page;
class Facebook;
class Post;
class Comment;
class Activity;
class Date;
class Memory;

template<class T>
class deAllocation
{
public:
	static void deallocate(T*& ptr)
	{
		if (ptr != NULL)
		{
			delete[] ptr;
			ptr = NULL;
		}
	}
	static void deallocate2D(T**& ptr, int size)
	{
		for (int i = 0; i < size; i++)
		{
			if (ptr[i] != NULL)
			{
				delete ptr[i];
				ptr[i] = NULL;
			}
		}
		delete[] ptr;
		ptr = NULL;
	}
};

class Helper
{
public:
	static int StringLength(char* str)
	{
		int i = 0;
		while (str[i] != '\0')
		{
			++i;
		}
		return i;
	}
	static void StringCopy(char*& dest, char*& source)
	{
		int length = StringLength(source);
		for (int i = 0; i < length; i++)
		{
			dest[i] = source[i];
		}
		dest[length] = '\0';
	}
	static char* GetStringFromBuffer(char* str)
	{
		int len = StringLength(str);
		char* stringNew = new char[len + 1];
		StringCopy(stringNew, str);
		return stringNew;
	}
	static int compareString(char* s1, char* s2)
	{
		int length = Helper::StringLength(s1);
		for (int i = 0; i < length; i++)
		{
			if (s1[i] != s2[i])
			{
				return 0;
			}
		}
		return 1;
	}
	static int numberOfFriends(char** tempList)
	{
		int number = 0;
		for (int i = 0; i < 4; i++)
		{
			if (tempList[i] == 0)
			{
			}
			else
				++number;
		}
		return number;
	}
};

class Date
{
	int day{ 0 };
	int mon{ 0 };
	int year{ 0 };
public:
	static Date currentSysDate;
	Date()
	{

	}
	Date(Date& d)
	{
		day = d.day;
		mon = d.mon;
		year = d.year;
	}
	void printDate()
	{
		cout << day << " / " << mon << " / " << year;
	}
	void readDate(ifstream& file)
	{
		file >> day;
		file >> mon;
		file >> year;
	}
	void setCurrentSysDate(int d, int m, int y)
	{
		currentSysDate.day = d;
		currentSysDate.mon = m;
		currentSysDate.year = y;
	}
	bool isLatest()
	{
		if (day == currentSysDate.day || day == currentSysDate.day - 1 && mon == currentSysDate.mon || mon == currentSysDate.mon - 1 && year == currentSysDate.year || year == currentSysDate.year - 1)
		{
			return true;
		}
		else
			false;
	}
	int getDay()
	{
		return day;
	}
	int getMon()
	{
		return mon;
	}
	int getYear()
	{
		return year;
	}
	friend ostream& operator<< (ostream&, Date&);
	friend int yearDiff(Date&, Date&);
};

int yearDiff(Date& d1, Date& d2)
{
	return (d1.year - d2.year);
}

ostream& operator<< (ostream& out, Date& date)
{
	out << '(' << date.day << ',' << date.mon << ',' << date.year << ')';
	return out;
}

Date Date::currentSysDate;

class Object
{
protected:
	char* ID{ NULL };

public:
	virtual void printName() {};
	virtual void printID() {};
	virtual char* getID() = 0;
	virtual void addPostToTimeline(Post*) {};
	virtual char* getName() = 0;;
};

class Comment
{
	char* ID{ NULL };
	char* Text{ NULL };
	Object* commentBy{ NULL };
public:
	~Comment()
	{
		deAllocation<char>::deallocate(ID);
		deAllocation<char>::deallocate(Text);
	}

	void setComment(char* id, char* txt, Object* cBy)
	{
		ID = id;
		Text = txt;
		commentBy = cBy;
	}

	void printComment()
	{
		cout << "\t\t\t"; commentBy->printName(); cout << " wrote : " << '"' << Text << '"' << ". " << endl;
	}

	void setNewComment(char* id, char* txt, Object* cBy)
	{
		ID = id;
		Text = txt;
		commentBy = cBy;
	}

};

class Activity
{
	int Type{ 0 };
	char* Value{ NULL };

public:
	~Activity()
	{
		deAllocation<char>::deallocate(Value);
	}
	void readData(ifstream& file)
	{
		file >> Type;
		char buffer[100];
		file.getline(buffer, 100, '\n');
		Value = Helper::GetStringFromBuffer(buffer);
	}
	void print()
	{
		if (Type == 1)
		{
			cout << " is feeling " << Value << endl;
		}
		if (Type == 2)
		{
			cout << " is thinking about " << Value << endl;
		}
		if (Type == 3)
		{
			cout << " is making " << Value << endl;
		}
		if (Type == 4)
		{
			cout << " is celebrating " << Value << endl;
		}
	}
};

class Post
{
	char* ID{ NULL };
	char* Text{ NULL };
	Comment** Comments{ new Comment * [10] {0,0,0,0,0,0,0,0,0,0} };
	Object* sharedBy{ NULL };
	Object** likedBy{ new Object * [10] {0,0,0,0,0,0,0,0,0,0} };
	Activity* activity{ NULL };
	Date sharedDate;
	int likeCount{ 0 };
	int commentCount{ 0 };

public:
	void setText(char* txt)
	{
		Text = txt;
	}
	char* getText()
	{
		return Text;
	}
	bool checkMem()
	{
		if (sharedDate.getDay() == Date::currentSysDate.getDay() && sharedDate.getMon() == Date::currentSysDate.getMon() && sharedDate.getYear() != Date::currentSysDate.getYear())
		{
			return true;
		}
		else
			return false;
	}
	char* getSharedByName()
	{
		return sharedBy->getName();
	}
	~Post()
	{
		deAllocation<char>::deallocate(ID);
		deAllocation<char>::deallocate(Text);
		if (activity != NULL)
		{
			delete activity;
			activity = 0;
		}
		delete[] likedBy;
		likedBy = 0;

		deAllocation<Comment>::deallocate2D(Comments, commentCount);
	}
	void setDate(Date& d)
	{
		sharedDate = d;
	}

	Date& getDate()
	{
		return sharedDate;
	}
	void setPost(char* id, char* txt, Object* sBy)
	{
		ID = id;
		Text = txt;
		sharedBy = sBy;
	}
	virtual void printPost()
	{
		if (activity != NULL)
		{
			sharedBy->printName();
			activity->print();
			cout << '"' << Text << '"' << "........" << sharedDate << endl;
			for (int i = 0; i < commentCount; i++)
			{
				Comments[i]->printComment();
			}
		}
		else
		{
			sharedBy->printName();
			cout << '"' << Text << '"' << "........" << sharedDate << endl;
			for (int i = 0; i < commentCount; i++)
			{
				Comments[i]->printComment();
			}
		}
	}

	void addNewComment(char* id, char* text, Object* ptr)
	{
		Comments[commentCount] = new Comment;
		Comments[commentCount]->setNewComment(id, text, ptr);
		++commentCount;
	}

	void printLikedBy()
	{
		cout << "Command: \t\t" << "ViewLikedList : " << getID();
		cout << "\n\nPost Liked By : \n";
		for (int i = 0; i < likeCount; i++)
		{
			likedBy[i]->printID();
			cout << " - ";
			likedBy[i]->printName();
			cout << endl;
		}
	}

	void printLatestPost()
	{
		if (sharedDate.isLatest() == true)
		{
			if (activity != NULL)
			{
				cout << "---";
				sharedBy->printName();
				activity->print();
				cout << '"' << Text << '"' << endl;
				for (int i = 0; i < commentCount; i++)
				{
					Comments[i]->printComment();
				}
			}
			else
			{
				cout << "---";
				sharedBy->printName();
				cout << " shared " << '"' << Text << '"' << endl;
				for (int i = 0; i < commentCount; i++)
				{
					Comments[i]->printComment();
				}
			}
		}
	}
	void addComment(Comment* comm)
	{
		Comments[commentCount] = comm;
		++commentCount;
	}

	static int totalPosts;

	char* getID()
	{
		return ID;
	}
	void readData(ifstream& file)
	{
		char buffer[100];
		int postType = 0;
		file >> postType;
		if (postType == 2)
		{
			activity = new Activity;
			file >> buffer;
			ID = Helper::GetStringFromBuffer(buffer);
			sharedDate.readDate(file);
			file.ignore();
			char tempSen[100];
			file.getline(tempSen, 100, '\n');
			Text = Helper::GetStringFromBuffer(tempSen);
			activity->readData(file);
		}
		if (postType == 1)
		{
			file >> buffer;
			ID = Helper::GetStringFromBuffer(buffer);
			sharedDate.readDate(file);
			file.ignore();
			char tempSen[100];
			file.getline(tempSen, 100, '\n');
			Text = Helper::GetStringFromBuffer(tempSen);
		}
	}
	char* getSharedByID()
	{
		return sharedBy->getID();
	}
	void setSharedBy(Object* ptr)
	{
		sharedBy = ptr;
	}
	void addLikedBy(Object* ptr)
	{
		likedBy[likeCount] = ptr;
		++likeCount;
	}
};

class Memory : public Post
{
	Post* originalPost{ NULL };
	char* shareText{ NULL };

public:
	Memory(Post* p, char* txt, Object* sharedBy)
	{
		originalPost = p;
		shareText = txt;
		setSharedBy(sharedBy);
		setDate(Date::currentSysDate);
	}
	void printPost()
	{
		cout << "~~~~~~~~" << getSharedByName() << " shared a memory ~~~~~~~~";
		cout << getDate();
		cout << endl << '"' << shareText << '"' << endl;
		cout << "\t\t(" << yearDiff(this->getDate(), originalPost->getDate()) << " Years Ago)\n\n";
		originalPost->printPost();
	}
};

int Post::totalPosts = 0;


class Page : public Object
{
public:
	static int numberOfPages;

private:
	char* Title{ NULL };
	Post** Timeline{ new Post * [10] {0,0,0,0,0,0,0,0,0,0} };
	int countPosts{ 0 };

public:
	char* getName()
	{
		return Title;
	}
	void printFullPage()
	{
		cout << ID << " - " << Title << endl;
		for (int i = 0; i < countPosts; i++)
		{
			Timeline[i]->printPost();
			cout << endl << endl;
		}
	}
	void printName()
	{
		cout << Title;
	}
	void printID()
	{
		cout << ID;
	}
	void addPostToTimeline(Post* post)
	{
		Timeline[countPosts] = post;
		++countPosts;
	}
	~Page()
	{
		deAllocation<char>::deallocate(ID);
		deAllocation<char>::deallocate(Title);
		for (int i = 0; i < countPosts; i++)
		{
			Timeline[i] = NULL;
		}
		delete[] Timeline;
		Timeline = NULL;
	}
	void printPage()
	{
		cout << ID << " - " << Title << endl;
	}
	void ReadData(ifstream& pageFile)
	{
		char temp[10];
		pageFile >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		char tempSen[100];
		pageFile.getline(tempSen, 100);
		Title = Helper::GetStringFromBuffer(tempSen);
	}
	char* getID()
	{
		return ID;
	}
	void printLatestPost()
	{
		for (int i = 0; i < countPosts; i++)
		{
			Timeline[i]->printLatestPost();
		}
	}
};

int Page::numberOfPages = 0;

class User : public Object
{
public:
	static int totalUsers;

private:
	char* firstName{ NULL };
	char* lastName{ NULL };
	User** friendList{ NULL };
	Page** likedPages{ NULL };
	int countPages{ 0 };
	int countFriends{ 0 };
	int countPosts{ 0 };
	Post** Timeline{ new Post * [10] {0,0,0,0,0,0,0,0,0,0} };
public:
	~User()
	{
		deAllocation<char>::deallocate(ID);
		deAllocation<char>::deallocate(firstName);
		deAllocation<char>::deallocate(lastName);

		delete[] friendList;
		friendList = 0;
		delete[] likedPages;
		likedPages = 0;

		deAllocation<Post>::deallocate2D(Timeline, countPosts);
	}
	void seeYourMemories()
	{
		cout << "We hope you enjoy looking back and sharing your memories of Facebook, from the most recent to those long ago" << endl;
		cout << "On this day" << endl;
		for (int i = 0; i < countPosts; i++)
		{
			if (Timeline[i]->checkMem())
			{
				Timeline[i]->printPost();
			}
		}
	}
	char* getName()
	{
		return firstName;
	}

	void shareMemory(char* txt, Post* p)
	{
		Memory* mem = new Memory{ p,txt,this };
		addPostToTimeline(mem);
	}
	void showTimeline()
	{
		cout << "\n--------------------------------------------------------------------------------------\n\n";
		this->printName(); cout << " - Timeline\n\n";
		for (int i = 0; i < countPosts; i++)
		{
			Timeline[i]->printPost();
			cout << endl;
		}
		cout << "\n--------------------------------------------------------------------------------------\n\n";
	}
	void likeAPost(Post* postID)
	{
		Timeline[countPosts] = postID;
		++countPosts;
		postID->addLikedBy(this);
	}
	void printName()
	{
		cout << firstName << " " << lastName << " ";
	}
	void printID()
	{
		cout << ID;
	}
	void printHome()
	{
		cout << "--------------------------------------------------------------------------------------\n";
		printName(); cout << " - Home Page\n\n";
		for (int i = 0; i < countFriends; i++)
		{
			friendList[i]->printLatestPost();
			cout << endl << endl;
		}

		for (int i = 0; i < countPages; i++)
		{
			likedPages[i]->printLatestPost();
			cout << endl;
		}
		cout << "\n--------------------------------------------------------------------------------------\n";
	}
	void printLatestPost()
	{
		for (int i = 0; i < countPosts; i++)
		{
			Timeline[i]->printLatestPost();
		}
	}
	void addPostToTimeline(Post* post)
	{
		Timeline[countPosts] = post;
		++countPosts;
	}
	char* getID()
	{
		return ID;
	}

	void printUser()
	{
		cout << "\nCommand : \tView Friend List\n\n";
		cout << "--------------------------------------------------------------------------------------\n\n";
		cout << firstName << " " << lastName << " -- Friend List\n\n";
		for (int i = 0; i < this->countFriends; i++)
		{
			friendList[i]->printSolo();
		}
		cout << "\n--------------------------------------------------------------------------------------\n\n";
		cout << "Command : \tView Liked Pages\n\n";
		cout << "--------------------------------------------------------------------------------------\n\n";
		cout << firstName << " " << lastName << " -- Liked Pages\n\n";
		for (int i = 0; i < this->countPages; i++)
		{
			likedPages[i]->printPage();
		}
	}
	void printSolo()
	{
		cout << ID << " - " << firstName << " " << lastName << endl;
	}
	void readData(ifstream& fin)
	{
		char temp[100];
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		fin >> temp;
		firstName = Helper::GetStringFromBuffer(temp);
		fin >> temp;
		lastName = Helper::GetStringFromBuffer(temp);
	}
	void likePage(Page* page)
	{
		countPages = countPages + 1;
		if (countPages > 1)
		{
			likedPages = makeCopyPages(likedPages);
			likedPages[countPages - 1] = page;
		}
		else if (countPages == 1)
		{
			likedPages = new Page* { page };
		}

	}
	void addFriend(User* user)
	{
		countFriends = countFriends + 1;
		if (countFriends > 1)
		{
			friendList = makeCopyUsers(friendList);
			friendList[countFriends - 1] = user;
		}
		else if (countFriends == 1)
		{
			friendList = new User* { user };
		}
	}

	Page** makeCopyPages(Page** prev)
	{
		Page** newLikedPages = new Page * [countPages];
		for (int i = 0; i < countPages - 1; i++)
		{
			newLikedPages[i] = prev[i];
		}
		return newLikedPages;
	}

	User** makeCopyUsers(User** prev)
	{
		User** newFriendList = new User * [countFriends];
		for (int i = 0; i < countFriends - 1; i++)
		{
			newFriendList[i] = prev[i];
		}
		return newFriendList;
	}

};

int User::totalUsers = 0;

class Facebook
{
private:
	User** Users{ NULL };
	Page** Pages{ NULL };
	Post** Posts{ NULL };

public:
	~Facebook()
	{
		deAllocation<User>::deallocate2D(Users, User::totalUsers);
		deAllocation<Page>::deallocate2D(Pages, Page::numberOfPages);

		delete[] Posts;
		Posts = 0;
	}
	void Run()
	{
		cout << "Command :\t\t" << "Set Current Date : 15-11-2017\n";
		Date::currentSysDate.setCurrentSysDate(15, 11, 2017);

		char userID[] = "u7";
		char postID[] = "post5";

		cout << "--------------------------------------------------------------------------------------\n";
		cout << "Command: \tSet Current User to: " << userID << '\n';
		cout << "--------------------------------------------------------------------------------------\n";
		User* currentUser;
		currentUser = searchUserByID(userID);
		currentUser->printName(); cout << " Successfully set as current user.\n";
		currentUser->printUser();

		cout << "--------------------------------------------------------------------------------------\n";
		cout << "\nCommand: \tView home\n\n";
		currentUser->printHome();

		cout << "\nCommand: \tView timeline\n\n";
		cout << "--------------------------------------------------------------------------------------\n\n";
		currentUser->printName(); cout << "-Timeline:\n";
		currentUser->showTimeline();

		Post* currentPost = searchPostByID(postID);
		cout << "Command: \tView Liked List(" << postID << ")\n\n";
		cout << "--------------------------------------------------------------------------------------\n";
		currentPost->printLikedBy();
		cout << "--------------------------------------------------------------------------------------\n";


		cout << "Command: \tLike Post(" << postID << ")\n";
		currentUser->likeAPost(currentPost);
		cout << "\nCommand: \tView Liked List(" << postID << ")\n";
		cout << "--------------------------------------------------------------------------------------\n";
		currentPost->printLikedBy();
		cout << "--------------------------------------------------------------------------------------\n";


		char post[6] = "post4";
		char* text = new char[50]{ "Good Luck for your Result (2)" };
		char* commentID = new char[4]{ "c14" };

		cout << "Command: \tPost Comment(" << post << ", " << text << ")\n";
		Post* commentOn = searchPostByID(post);
		commentOn->addNewComment(commentID, text, currentUser);
		cout << "Command: \tView Post(" << post << ")\n\n";
		commentOn->printPost();
		cout << "--------------------------------------------------------------------------------------\n";

		char post2[6] = "post8";
		char* text2 = new char[50]{ "Thanks for the wishes" };
		char* commentID2 = new char[4]{ "c15" };
		cout << "Command: \tPost Comment(" << post2 << ", " << text2 << ")\n";
		Post* commentOn2 = searchPostByID(post2);
		commentOn2->addNewComment(commentID2, text2, currentUser);
		cout << "Command: \tView Post(" << post2 << ")\n\n";
		commentOn2->printPost();
		cout << "--------------------------------------------------------------------------------------\n";

		cout << "Command: \tSeeYourMemories()\n";
		currentUser->seeYourMemories();
		cout << "--------------------------------------------------------------------------------------\n";

		char mPost[7] = "post10";
		currentPost = searchPostByID(mPost);
		char shareText[70] = "Never thought I will be specialist in this field...";
		cout << "Command: \tShareMemory(" << mPost << ",\"" << shareText << "\")\n";
		currentUser->shareMemory(shareText, currentPost);
		cout << "\nCommand: \tView timeline\n";
		currentUser->printName(); cout << "-Timeline:\n\n";
		currentUser->showTimeline();
		cout << "--------------------------------------------------------------------------------------\n";

		char viewPage[] = "p1";
		Page* toView = searchPageByID(viewPage);
		cout << "Command :\tViewPage(" << viewPage << ")\n\n";
		toView->printFullPage();
		cout << "--------------------------------------------------------------------------------------\n";
	}
	void printAll()
	{
		for (int i = 0; i < 20; i++)
		{
			Users[i]->printUser();
		}
	}
	void GetData(const char* users, const char* pages, const char* posts, const char* comments)
	{
		ifstream UserFile;
		ifstream PageFile;
		ifstream PostFile;
		ifstream CommentFile;

		PageFile.open(pages);
		this->loadPages(PageFile);
		PageFile.close();

		UserFile.open(users);
		this->loadUsers(UserFile);
		UserFile.close();

		PostFile.open(posts);
		this->loadPosts(PostFile);
		PostFile.close();

		CommentFile.open(comments);
		this->loadComments(CommentFile);
		CommentFile.close();

	}
	void loadComments(ifstream& comFile)
	{
		int totalComments = 0;
		comFile >> totalComments;

		Comment** tempComments = new Comment * [totalComments];

		for (int i = 0; i < totalComments; i++)
		{
			tempComments[i] = new Comment;
			char* id, * postid, * userid, * text;
			char buffer[100];
			comFile >> buffer;
			id = Helper::GetStringFromBuffer(buffer);
			comFile >> buffer;
			postid = Helper::GetStringFromBuffer(buffer);
			comFile >> buffer;
			userid = Helper::GetStringFromBuffer(buffer);
			comFile.getline(buffer, 100, '\n');
			text = Helper::GetStringFromBuffer(buffer);

			if (userid[0] == 'u')
			{
				Object* obj = searchUserByID(userid);
				tempComments[i]->setComment(id, text, obj);
			}
			if (userid[0] == 'p')
			{
				Object* obj = searchPageByID(userid);
				tempComments[i]->setComment(id, text, obj);
			}

			Post* post = searchPostByID(postid);
			post->addComment(tempComments[i]);
		}
	}

	void loadPosts(ifstream& postFile)
	{
		int total = 0;
		postFile >> total;
		Post::totalPosts = total;
		Posts = new Post * [total];

		for (int i = 0; i < total; i++)
		{
			Posts[i] = new Post;
			Posts[i]->readData(postFile);
			char buffer[50];

			postFile >> buffer;

			if (buffer[0] == 'u')
			{
				char* ID = Helper::GetStringFromBuffer(buffer);
				Object* ptr = searchUserByID(ID);
				Posts[i]->setSharedBy(ptr);
				ptr->addPostToTimeline(Posts[i]);
			}
			if (buffer[0] == 'p')
			{
				char* ID = Helper::GetStringFromBuffer(buffer);
				Object* ptr = searchPageByID(ID);
				Posts[i]->setSharedBy(ptr);
				ptr->addPostToTimeline(Posts[i]);
			}

			postFile >> buffer;

			while (buffer[0] != '-')
			{
				if (buffer[0] == 'u')
				{
					char* ID = Helper::GetStringFromBuffer(buffer);
					Object* ptr = searchUserByID(ID);
					Posts[i]->addLikedBy(ptr);
					postFile >> buffer;
				}
				if (buffer[0] == 'p')
				{
					char* ID = Helper::GetStringFromBuffer(buffer);
					Object* ptr = searchPageByID(ID);
					Posts[i]->addLikedBy(ptr);
					postFile >> buffer;
				}
			}

		}
	}
	void loadPages(ifstream& pagesFile)
	{
		int totalPages = 0;
		pagesFile >> totalPages;
		Page::numberOfPages = totalPages;
		Pages = new Page * [totalPages];
		for (int i = 0; i < totalPages; i++)
		{
			Pages[i] = new Page;
			Pages[i]->ReadData(pagesFile);
		}
	}
	void loadUsers(ifstream& userFile)
	{
		int totalUsers = 0;
		char*** tempFriendList = NULL;
		userFile >> totalUsers;
		User::totalUsers = totalUsers;
		Users = new User * [totalUsers];
		tempFriendList = new char** [totalUsers];

		for (int i = 0; i < totalUsers; i++)
		{
			Users[i] = new User;
			Users[i]->readData(userFile);

			char* buffer = new char[80];

			tempFriendList[i] = new char* [10];

			for (int j = 0; j < 10; j++)
			{
				tempFriendList[i][j] = 0;
			}

			userFile >> buffer;

			for (int j = 0; buffer[0] != '-'; j++)
			{
				tempFriendList[i][j] = Helper::GetStringFromBuffer(buffer);
				userFile >> buffer;
			}

			userFile >> buffer;

			while (buffer[0] != '-')
			{
				char* pageID = Helper::GetStringFromBuffer(buffer);
				Page* p = searchPageByID(pageID);
				Users[i]->likePage(p);
				userFile >> buffer;
			}
		}

		for (int i = 0; i < totalUsers; i++)
		{
			int noOfFriends = Helper::numberOfFriends(tempFriendList[i]);
			for (int j = 0; j < noOfFriends; j++)
			{
				User* friendID = searchUserByID(tempFriendList[i][j]);
				Users[i]->addFriend(friendID);
			}
		}

		for (int i = 0; i < totalUsers; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				delete[] tempFriendList[i][j];
				tempFriendList[i][j] = NULL;
			}
			delete[] tempFriendList[i];
			tempFriendList[i] = NULL;
		}
		delete[] tempFriendList;
		tempFriendList = NULL;

	}
	Page* searchPageByID(char* pageID)
	{
		for (int i = 0; i < Page::numberOfPages; i++)
		{
			if (Helper::compareString(pageID, Pages[i]->getID()) == 1)
			{
				return Pages[i];
			}
		}
	}
	User* searchUserByID(char* userID)
	{
		for (int i = 0; i < User::totalUsers; i++)
		{
			if (Helper::compareString(userID, Users[i]->getID()) == 1)
			{
				return Users[i];
			}
		}
	}
	Post* searchPostByID(char* postID)
	{
		for (int i = 0; i < Post::totalPosts; i++)
		{
			if (Helper::compareString(postID, Posts[i]->getID()) == 1)
			{
				return Posts[i];
			}
		}
	}
};

int main()
{
	Facebook FB;
	FB.GetData("Users.txt", "Pages.txt", "Posts.txt", "Comments.txt");
	FB.Run();
	system("pause");
}
