
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <map>

using std::cout;
using std::endl;

class Car{
private:
	int id;
public:
	Car(int a = 0) : id(a) {};
	~Car() { cout << "car[" << id << "] delete" << endl; };
	void show() { cout << "car.id=" << id << endl; };
	int getId() { return id; };
};

// bad case
void func1()
{
	Car * c = new Car(10);
	c->show();
	if (1) {
		throw "func1_throw";
	}

	delete c; // will never get here, memory leak
}

void case1() 
{
	try {
		func1();
	} catch (const char *) {
	}

}


void func2()
{
	std::auto_ptr<Car> c (new Car(20)); // auto_ptr constructor
	c->show();
	if (1) {
		throw "func2_throw";
	}

	// delete c; // no need to delete smart ptr
}

// std::auto_ptr case
void case2() 
{
	try {
		func2();
	} catch (const char *) {
	}

	std::auto_ptr<Car> car1(new Car(21));
	car1->show();
	std::auto_ptr<Car> car2;
	car2 = car1; // ownership will passed to car2
	car2->show();
	// car->show(); // this will bug! car1 cannot be used again!


}

void func3()
{
	std::unique_ptr<Car> c (new Car(30));
	c->show();
	if (1) {
		throw "func3_throw";
	}

	// delete c; // no need to delete smart ptr
}

std::unique_ptr<Car> clone_car(Car c)
{
	return std::unique_ptr<Car>(new Car(c.getId()));
}

void self_delete(Car *car)
{
	cout << "self_delete:" << endl;
	car->show();	
}

// std::unique_ptr case
void case3() 
{
	try {
		func3();
	} catch (const char *) {
	}

	// only way to init
	std::unique_ptr<Car> car(new Car(31));
	car->show(); // "->" to call real ptr function, "." to call unique_ptr function

	// std::unique_ptr<Car> car2;
	// car2 = car; // unique_ptr cannot pass ownership, avoid auto_ptr problem

	std::unique_ptr<Car> car2(new Car(33));
	car2->show();
	car2.reset();

	std::unique_ptr<Car> car3;
	// car3 = new Car(32); // cannot pass value
	car3.reset(new Car(32)); // reset is ok
	car3->show();

	std::unique_ptr<Car> car4;
	car4.reset(car3.release()); // release control, return ptr

	std::unique_ptr<Car> car5(new Car(35));
	auto ptr = car3.release();
	delete ptr; // must delete it

	Car c(36);
	std::unique_ptr<Car> car6 = clone_car(c); // spacial copy
	car6->show();

	Car c2(37);
	std::unique_ptr<Car, decltype(self_delete)*> car7(&c2, self_delete);



}

void func4()
{
	std::shared_ptr<Car> c (new Car(40));
	c->show();
	if (1) {
		throw "func4_throw";
	}

	// delete c; // no need to delete smart ptr
}

// std::shared_ptr case
void case4() 
{
	try {
		func4();
	} catch (const char *) {
	}

	std::shared_ptr<Car> car(new Car(41));
	car->show();
	std::shared_ptr<Car> car2;
	car2 = car; // shared_ptr ok with this, because it has a counter inside
	car2->show();
	
	// make_shared
	auto car3 = std::make_shared<Car>(42);
	car3->show();

	// replace delete function (deleter)
	Car car4(43);
	std::shared_ptr<Car> p(&car4, self_delete);

#ifdef CPP11
	// shared_ptr handle list, have to init with delete function
	auto del_func = [](Car *list){ delete [] list; };
	std::shared_ptr<Car> car_list(new Car[10], del_func);
	// car_list[0].show(); // not support []... have to get inner pointer
#endif

}

std::unique_ptr<Car> func5()
{
	std::unique_ptr<Car> ptr(new Car(51));
	return ptr;
}

// unique_ptr special pass 
void case5()
{
	// unique_ptr can pass ownership by function return
	std::unique_ptr<Car> ptr = func5();	
	ptr->show();
}

void case6()
{
	// only unique_ptr can handle new[] and delete[]
	std::unique_ptr<int[]> uints(new int[5]);
	// std::shared_ptr<int[]> sints(new int[5]); // error, shared_ptr cannot handle new array
	cout << "uint[0]=" << uints[0] << endl;

	Car *cars = new Car[5]();
	std::unique_ptr<Car[]> ucars (cars);
	// or
	// std::unique_ptr<Car[]> ucars = (std::unique_ptr<Car[]>)cars;
	// std::unique_ptr<Car[]> ucars (new Car[5]());
	ucars[0].show();
	
}

class A1{
public:
    int p1;
    A1(int c=0) : p1(c) {};
    ~A1()
    {
        std::cout << "~A1() p1=" << p1 << std::endl;
    }
};

class A2{
public:
    int p2;

    std::list<std::shared_ptr<A1>> my_list;

    A2(int c=0) : p2(c) {};
    ~A2()
    {
        std::cout << "~A2() p2=" << p2 << std::endl;
    }
};

class A3{
public:
    int p3;

    std::map<int, std::shared_ptr<A2>> my_map;

    A3(int c=0) : p3(c) {};
    ~A3()
    {
        std::cout << "~A3() p3=" << p3 << std::endl;
    }
};

class A4{
public:
    int p4;

    std::vector<std::shared_ptr<A3>> my_vec;

    A4(int c=0) : p4(c) {};
    ~A4()
    {
        std::cout << "~A4() p4=" << p4 << std::endl;
    }
};

void case7()
{
    const int MAX_SIZE = 3;
    auto a4 = std::make_shared<A4>(0);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        auto a3 = std::make_shared<A3>(i);
        for (int j = 0; j < MAX_SIZE; j++)
        {
            auto a2 = std::make_shared<A2>(j);
            for (int k = 0; k < MAX_SIZE; k++)
            {
                auto a1 = std::make_shared<A1>(k);
                a2->my_list.push_back(a1);
            }
            a3->my_map.insert(std::make_pair(j,a2));
        }
        a4->my_vec.push_back(a3);
    }
	a4->p4 = 1;
}

std::shared_ptr<A1> getA1(int type)
{
    if (type == 0)
    {
        return std::shared_ptr<A1>();
    }
    return std::make_shared<A1>();
}

void case8()
{
    std::shared_ptr<A4> ptr;
    if (ptr)
    {
        std::cout << "ptr not empty" << std::endl;
    }
    else
    {
        std::cout << "ptr empty" << std::endl;
    }

    std::shared_ptr<A1> ptr2;
    ptr2 = getA1(0);
    if (ptr2)
    {
        std::cout << "ptr2 not empty" << std::endl;
    }
    else
    {
        std::cout << "ptr2 empty" << std::endl;
    }

    std::shared_ptr<A1> ptr3;
    ptr3 = getA1(1);
    if (ptr3)
    {
        std::cout << "ptr3 not empty" << std::endl;
    }
    else
    {
        std::cout << "ptr3 empty" << std::endl;
    }
}


int main(int argc, char *argv[])
{
	cout << "smart_pointer test" << endl;

	cout << "case1 ------------------------------" << endl;
	case1();
	cout << endl;

	cout << "case2 ------------------------------" << endl;
	case2();
	cout << endl;

	cout << "case3 ------------------------------" << endl;
	case3();
	cout << endl;

	cout << "case4 ------------------------------" << endl;
	case4();
	cout << endl;
	
	cout << "case5 ------------------------------" << endl;
	case5();
	cout << endl;
	
	cout << "case6 ------------------------------" << endl;
	case6();
	cout << endl;
	
	cout << "case7 ------------------------------" << endl;
	case7();
	cout << endl;
	
	cout << "case8 ------------------------------" << endl;
	case8();
	cout << endl;
	return 0;
}
