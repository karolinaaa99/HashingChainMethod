#include <iostream>
#include<fstream>

using namespace std;

class Elem //element listy dwukierunkowej
{
private:
	string s; //string, ciag
	Elem* next; //wskaznik na nastepnika
	Elem* prev; //wskaznik na poprzednika
public:
	Elem(); //konstruktor domyslny, tworzy pusty element
	Elem(string s, Elem* next, Elem* prev); //konstruktor, tworzy element okreslony
	string GetString(); //zwraca stringa
	Elem* GetNext(); //zwraca nastepnika
	void SetNext(Elem* next); //ustawia nastepnika
	Elem* GetPrev(); //zwraca poprzednika
	void SetPrev(Elem* prev); //ustawia poprzednika
};

Elem::Elem()
{
	this->s = "";
	this->next = NULL;
	this->prev = NULL;
}

Elem::Elem(string s, Elem* next, Elem* prev)
{
	this->s = s;
	this->next = next;
	this->prev = prev;
}

string Elem::GetString()
{
	return this->s;
}

Elem* Elem::GetNext()
{
	return this->next;
}

void Elem::SetNext(Elem* next)
{
	this->next = next;
}

Elem* Elem::GetPrev()
{
	return this->prev;
}

void Elem::SetPrev(Elem* prev)
{
	this->prev = prev;
}

class DLList //lista dwukierunkowa
{
private:
	Elem* head; //wskaznik na poczatek listy
public:
	DLList(); //konstruktor domyslny, tworzy pusta liste dwukierunkowa 
	void insertFirst(string s); //wstawia nowego stringa na poczatku listy, bo w tym haszowaniu zawsze wstawiamy nowy element na poczatku listy
	void del(string s); //usuwa stringa z listy
	string locate(string s); //zwraca s, jesli istnieje, zwraca pusty ciąg jeśli nie istnieje
	friend ostream& operator<<(ostream& out, DLList& l); //wypisuje całą listę
};

DLList::DLList()
{
	this->head = NULL;
}

void DLList::insertFirst(string s)
{
	if (head == NULL)
	{
		head = new Elem(s, NULL, NULL);
	}
	else
	{
		Elem* newElem = new Elem(s, head, NULL);
		head->SetPrev(newElem);
		head = newElem;
	}
}

void DLList::del(string s)
{
	Elem* temp = head;

	if (temp == NULL) //jesli lista jest pusta, to nie ma co usuwac
	{
		return;
	}

	while (temp->GetString() != s) //szukam stringa, ktorego mam usunac w liscie
	{
		temp = temp->GetNext();
		if (temp == NULL) //jesli next jest nullem, tzn., ze przeszlam cala liste i tego stringa nie ma
		{
			return;
		}
	}

	if (head == temp) //jesli elementem, ktory mam usunac jest head
		head = head->GetNext(); //ustawiam nowa head

	Elem* prev = temp->GetPrev(); //zapamietuje poprzednika temp
	Elem* next = temp->GetNext(); //zapamietuje nastepnika temp

	if (prev != NULL) //jesli poprzednik jest rozny od null
		prev->SetNext(next);
	if (next != NULL) //jesli nastepnik jest rozny od null
		next->SetPrev(prev);

	delete temp; //usuwam stringa
}

string DLList::locate(string s)
{
	Elem* temp = head;
	if (temp->GetString() == s) //jesli glowa jest tym stringiem
		return s;
	while (temp->GetString() != s)
	{
		temp = temp->GetNext();
		if (temp == NULL) //jesli przeszlam cala liste i tego stringa nie znalazlam
			return "";
	}
	return temp->GetString();
}

ostream& operator<<(ostream& out, DLList& l)
{
	Elem* temp = l.head;

	if (temp == NULL) 
	{
		out << endl;
		return out;
	}

	while (temp != NULL) 
	{
		out << temp->GetString() << " ";
		temp = temp->GetNext();
	}
	out << endl;
	return out;
	
	/**Elem* temp = l.head;

	if (temp == NULL) //jesli lista jest pusta
	{
		out << endl;
		return out;
	}

	while (temp != NULL)
	{
		out << temp->GetString() << " ";
		temp = temp->GetNext();
	}
	out << endl;
	return out;**/
}

class HashTable2 //tablica list dwukierunkowych
{
private:
	DLList* t; //tablica z haszowaniem, czyli tablica list dwukierunkowych
	int capacity; //maksymalna pojemnosc tablicy z haszowaniem
	int ht_size; //liczba elementow w tablicy z haszowaniem
public:
	HashTable2(int c); //konstruktor, tworzący pustą tablicę o pojemności c
	int hashF(string s); //funkcja haszująca dla ciągu s
	void insert(string s);	//wstawienie ciągu s
	void del(string s); //usuniecie ciagu s
	string search(string s);	//wyszukuje i zwraca s
	friend ostream& operator<<(ostream& out, HashTable2& ht); //wyswietla na ekranie tablice z haszowaniem
};

HashTable2::HashTable2(int c)
{
	capacity = c;
	t = new DLList[capacity];
	ht_size = 0;
}

int HashTable2::hashF(string s)
{
	unsigned long int wynik = 0;

	for (int n = 0; n < s.size(); n++) 
	{
		wynik += (unsigned)(s[n] * (int)pow(11, s.size() - n));
		wynik %= capacity;
	}

	return (int)wynik;
}

void HashTable2::insert(string s)
{
	int hash = hashF(s);
	t[hash].insertFirst(s);
	ht_size++;
}

void HashTable2::del(string s)
{
	int hash = hashF(s);
	if (t[hash].locate(s) == "")
	{
		return;
	}

	t[hash].del(s);
	ht_size--;
}

string HashTable2::search(string s)
{
	int hash = hashF(s);
	return t[hash].locate(s);
}

ostream& operator<<(ostream& out, HashTable2& ht)
{
	if (ht.ht_size == 0) 
	{
		return out;
	}
	else 
	{
		for (int i = 0; i < ht.capacity; i++) 
		{
			out << i << ": " << ht.t[i];
		}
	}
	out << endl;
	return out;
}

int main() 
{
	HashTable2 haszowanko(50);

	ifstream wejscie("dane.txt");
	string dane;

	if (wejscie.good()) 
	{
		cout << "Plik dane.txt otwarty" << endl;
	}

	for (int i = 0; i < 40; i++) 
	{
		wejscie >> dane;
		haszowanko.insert(dane);
	}

	cout << haszowanko << endl;

	haszowanko.del("Liliana");
	haszowanko.del("Igor");

	cout << haszowanko << endl;

	cout << "Search1: " << haszowanko.search("Jan") << endl;
	cout << "Search2: " << haszowanko.search("KEKW") << endl;

	//int cos = INT_MAX;
	return 0;
}