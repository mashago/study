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
            console.log('name:' + person.name + ' job:' + person.job + ' age:' + person.age);
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
        console.log('car=' + cars[index]);
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
    console.log(obj.name + " " + obj.age + " " + obj.cars);
}

function vaFunc() {
    var i, a = arguments[0];
    console.log(arguments);
    console.log(i + " " + a);
}

function test9() {
    vaFunc();
    vaFunc(1, 2, 3);
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
];

function main() {
    var c = 9;
    var func = funcList[c];
    func();
}
main();
