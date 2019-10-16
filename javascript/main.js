'use strict';

var fs = require('fs');

function test0() {
    console.log('hello javascript');
}

function createFile(path) {
    let data = 'hello javasrcipt';
    fs.writeFileSync(path, data);
}

function removeFile(path) {
    fs.unlinkSync(path);
}

function syncRead(path) {
    createFile(path);
    let data = fs.readFileSync(path);
    console.log(data.toString());
    removeFile(path);
}

function asyncRead(path) {
    createFile(path);
    fs.readFile(path, function (err, data) {
        if (err) {
            console.log(err);
        } else {
            console.log(data.toString());
        }
        removeFile(path);
    });
}

// file
function test1() {
    let fileName = 'input.txt';
    console.log('start read');
    syncRead(fileName);
    asyncRead(fileName);
    console.log('end read');
}

// module
function test2() {
    let moduleFunc = require('./module_func');
    moduleFunc.world();

    let World = require('./module_class');
    let world = new World();
    world.setName('masha');
    world.printName();
}

function makePerson() {
    let person = {
        name: 'masha',
        job: 'coder',
        age: 10,
        print: function () {
            // console.log('name:' + person.name + ' job:' + person.job + ' age:' + person.age);
            console.log('name:' + this.name + ' job:' + this.job + ' age:' + this.age);
        }
    }
    return person
}

// ES6 default params
function Person(name = 'no name', job = 'no job', age = 0) {
    // ES5
    // name = name || 'no name';
    // job = job || 'no job';
    // age = age || 0;

    this.name = name;
    this.job = job;
    this.age = age;
    this.print = function () {
        console.log('name:' + this.name + ' job:' + this.job + ' age:' + this.age);
    }
}
Person.tmpAttr1 = 111;
Person.prototype.tmpAttr2 = 222;

function objectFactory() {
    let obj = {};
    let Constructor = [].shift.call(arguments);
    obj.__proto__ = Constructor.prototype;
    let ret = Constructor.apply(obj, arguments);
    return (typeof ret === 'object' || typeof ret === 'function') ? ret : obj;
}

function changePersonAge(person) {
    person.age += 100
}

function test3() {
    let a = 1;
    a = 'hello';
    console.log(a);

    var b = 2;
    var b = 'world';
    console.log(b);

    let person = makePerson();
    console.log('name:' + person.name + ' job:' + person.job + ' age:' + person.age);
    person.print();
    console.log(person);
    person = null;
    console.log(person);

    let person2 = new Person('peter', 'cook', 100);
    person2.print();
    console.log(person2);

    let person3 = new Person();
    person3.print();
    console.log(person3);
    changePersonAge(person3);
    person3.print();
    console.log('tmpAttr1:' + person3.tmpAttr1);
    console.log('tmpAttr2:' + person3.tmpAttr2);
    person3.tmpAttr1 = 123;
    person3.tmpAttr1 = 123;
    console.log('tmpAttr1:' + person3.tmpAttr1);
    console.log('tmpAttr2:' + person3.tmpAttr2);

    // this way is call Person() by global, 'this' is global
    // let person4 = Person();
    // console.log(person4);

    let person5 = objectFactory(Person, 'kelton');
    person5.print();
    console.log();

    console.log('Person.name:' + Person.name);
    console.log('Person.job:' + Person.job);
    console.log('Person.tmpAttr2:' + Person.tmpAttr2);
    console.log('Person.__proto__:' + Person.__proto__)
    console.log('Person.prototype:' + Person.prototype)
    console.log(Person.__proto__ == Function.prototype);
    console.log(Function.prototype.__proto__ == Object.prototype);
    console.log(Object.prototype.__proto__);
}

function test4() {
    let cond = true;
    let a = (cond) ? 1 : 2;
    console.log('a=' + a);
}

function test5() {
    let d = new Date().getDay();
    switch (d) {
        case 0:
            console.log('sunday');
            break;
        case 6:
            console.log('saturday');
            break;
        default:
            console.log('workday');
    }
}

function test6() {
    for (let i=0; i<5; i++) {
        console.log('i=' + i);
    }
    console.log()

    let cars = new Array();
    cars[0] = 'honda';
    cars[1] = 'toyota';
    cars[2] = 'mazda';
    for (let index in cars) {
        console.log('index=' + index);
    }
    for (let car of cars) {
        console.log('car=' + car);
    }
    console.log()

    let person = makePerson();
    for (let x in person) {
        console.log('x=' + person[x]);
    }
    console.log()

    let cond = 0;
    while (cond < 5) {
        console.log('cond=' + cond);
        cond++;
    }
    console.log()

    wrap: {
        console.log('1');
        console.log('2');
        break wrap;
        console.log('3');
    }

    wrap: {
        console.log('11');
        console.log('12');
        break wrap;
        console.log('13');
    }
}

function test7() {
    console.log(typeof 1)
    console.log(typeof 'masha')
    console.log(typeof true)
    console.log(typeof ['a', 'b'])
    console.log(typeof { name: 'masha', age: 100 })
    console.log(typeof {})
    console.log(typeof null)
    console.log(typeof undefined)
    console.log(typeof function () {
    })
    console.log()

    let person = makePerson();
    console.log(person + ' : ' + typeof person)
    person = null
    console.log(person + ' : ' + typeof person)
    person = undefined
    console.log(person + ' : ' + typeof person)

    if (null == undefined) {
        console.log('null == undefined')
    }
    if (null !== undefined) {
        console.log('null !== undefined')
    }
}

function test8() {
    let text = '{ \
        "name": "masha", \
        "age": "100", \
        "cars": [ \
            "honda", \
            "mazda" \
        ] \
    }';
    let obj = JSON.parse(text);
    console.log(obj.name + ' ' + obj.age + ' ' + obj.cars);
}

function vaFunc() {
    let i, a = arguments[0];
    console.log(arguments);
    console.log(i + ' ' + a);
}

function test9() {
    vaFunc();
    vaFunc(1, 2, 3);
}

// closure
function createCounter() {
    let count = 0;
    return function() {
        return ++count;
    }
}

function test10() {
    let counter1 = createCounter();
    console.log('counter1:' + counter1());
    console.log('counter1:' + counter1());
    console.log('counter1:' + counter1());

    let counter2 = createCounter();
    console.log('counter2:' + counter2());
    console.log('counter1:' + counter1());
    console.log('counter2:' + counter2());
}

function test11() {
    let d = new Date();
    console.log(d);
    console.log(d.getTime());
    console.log(new Date(2019, 10, 1, 8, 30, 0));
    let localOffset = d.getTimezoneOffset();
    console.log(localOffset);
}

function test12() {
    let cars = ['honda', 'toyota', 'mazda'];
    console.log(cars.length);
    console.log(cars.indexOf('toyota'));
    console.log(cars.indexOf('volvo'));

    cars.splice(1, 1, 'volvo');
    console.log(cars);

    cars.splice(1, 1);
    console.log(cars);

    cars.splice(1, 0, 'bmw');
    console.log(cars);

    cars.push('benz');
    console.log(cars);

    cars.pop();
    console.log(cars);

    let cars2 = ['byd', 'hongqi'];
    let cars3 = cars.concat(cars2);
    console.log(cars3);

    let c = [].shift.call(cars3);
    console.log('c:' + c + ' cars3:' + cars3);
}

function test13() {
    const f = (x, y) => x * y;
    console.log(f(2, 3));

    const f2 = () => 2 * 2;
    console.log(f2());
}

function test14() {
    let t = new Map();
    t.set('apple', 1000);
    t.set(1000, 'apple');
    t.set('pear', 3000);
    console.log(t);
    console.log(t.has('apple'));
    console.log(t.get('apple'));
    t.set('apple', 2000);
    console.log(t.get('apple'));
    console.log(t.delete('apple'));
    console.log(t.get('apple'));

    let s = new Set();
    s.add(1);
    s.add(2);
    s.add(3);
    s.add(1);
    console.log(s);
    s.delete(1);
    console.log(s);
    console.log();

    let a = new Array();
    a.push('a');
    a.push('b');
    a.push('c');
    console.log(a);
    console.log();

    for (let p of t) {
        console.log(p);
        console.log(p[0]);
        console.log(p[1]);
    }
    console.log();

    for (let v of s) {
        console.log(v);
    }
    console.log();

    for (let v of a) {
        console.log(v);
    }
    console.log();

    a.forEach((value, index, arr) => {
        console.log('[' + index + ']:' + value);
    });
    console.log();

    a.every((value, index, arr) => {
        console.log('[' + index + ']:' + value);
        if (value == 'b') return false; // false for break
        return true; // true for continue
    });
    console.log();

    console.log(a.some((value, index, arr) => {
        return value == 'b';
    }));
}

function test15() {
    let data = fs.readFileSync('skeleton.json');
    let obj = JSON.parse(data);
    console.log(obj);
}

let funcList = [
    test0,
    test1,
    test2,
    test3,
    test4,
    test5,
    test6,
    test7,
    test8,
    test9,
    test10,
    test11,
    test12,
    test13,
    test14,
    test15,
];

function main() {
    var c = 15;
    var func = funcList[c];
    func();
}
main();
