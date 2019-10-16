
function test0(): void {
    var message:string = "hello typescript";
    console.log(message);
}

function loop(): never {
    while (true) {}
}

function union(a: number|string): void {
    console.log(a);
}

function test1(): void {
    let a: number = 1;
    let b: string = 'abc';
    let c: string = `a:${a} b:${b}`;
    console.log(c);
    let d: boolean = true;
    let e: number[] = [1, 2];
    let f: Array<number> = [];
    let g: string[];
    let h: [string, number];
    h = ['hello', 1];
    enum Color {Red, Green, Blue};
    let i: Color = Color.Red;
    console.log(i);

    let j: any = 1;
    j = 'hello';
    j = false;

    let k: any[] = [1, 'hello'];

    let l: never;
    // l = loop();

    let m = 1;
    console.log(typeof m);
    let n;
    console.log(typeof n);

    // let o: number = 'masha';

    let p = '1';
    let q:number = <number><any>p;
    console.log(typeof q);

    let r: number|string;
    r = 1;
    r = 'abc';
    console.log(r);

    union(123);
    union('jqk');
}

class Car {
    brand = 'no brand';
    price = 0;
    static maxSpeed = 180;
    setBrand(brand): void {
        this.brand = brand;
    }

    setPrice(price): void {
        this.price = price;
    }

    print(): void {
        console.log(`brand:${this.brand} price:${this.price}`);
    }
}

function test2(): void {
    var car = new Car();
    car.setBrand('honda');
    car.setPrice(1000);
    car.print();
}

function addNum(a: number, b: number): number {
    return a + b;
}

function addNum2(a: number, b?: number): number {
    if (b === undefined) b = 0;
    return a + b;
}

function addNum3(a: number, b: number = 100): number {
    return a + b;
}

function addNum4(...nums: number[]): number {
    let n = 0;
    for (let i = 0; i < nums.length; ++i) {
        n = n + nums[i];
    }
    return n;
}

function addNum5(a: number = 0, ...nums: number[]): number {
    let n = a;
    for (let i = 0; i < nums.length; ++i) {
        n = n + nums[i];
    }
    return n;
}

function factorial(num) {
    if (num <= 0) {
        return 1;
    } else {
        return (num * factorial(num - 1));
    }
}

function test3() {
    console.log(addNum(3, 4));
    console.log(addNum2(3));
    console.log(addNum3(3));
    console.log(addNum4(1, 2, 3, 4));
    console.log(addNum5(1, 2, 3, 4));

    console.log((function(): number {
        return 1;
    })());
    console.log(factorial(6));
}

interface ICar {
    brand: string;
    price: number;
    print: () => void;
}

function test4() {
    let a: ICar = {
        brand: 'honda',
        price: 1000,
        print: ():void => {
            // not the way you think like javascript, will use global as this, so shit
            console.log(`brand:${this.brand} price:${this.price}`);
        }
    }
    a.print();
}

class Moto {
    brand: string;
    price: number;

    constructor(brand: string, price: number)
    {
        this.brand = brand;
        this.price = price;
    }

    print(): void {
        console.log(`brand:${this.brand} price:${this.price}`);
    }
}

class SuperMoto extends Moto {
    power: number;

    constructor(brand: string, price: number, power: number)
    {
        super(brand, price);
        this.power = power;
    }

    print(): void {
        super.print();
        console.log(`power:${this.power}`);
    }

    static maxPower: number = 100000;
    static printMaxPower(): void {
        console.log( SuperMoto.maxPower);
    }

    attrA: number = 100;
    public attrB: number = 200;
    private attrC: number = 300;
    getAttrC(): number {
        return this.attrC;
    }
}

interface IPlane {
    maxSpeed: number;
}

class Plane implements IPlane {
    maxSpeed: number;
    speed: number;
    constructor(maxSpeed: number, speed: number) {
        this.maxSpeed = maxSpeed;
        this.speed = speed;
    }
}

function test5() {
    let c = new Moto('honda', 1000);
    c.print();
    let d = new SuperMoto('honda', 2000, 999);
    d.print();
    console.log(SuperMoto.maxPower);
    SuperMoto.printMaxPower();

    console.log(c instanceof Moto);
    console.log(d instanceof Moto);
    console.log(d instanceof SuperMoto);
    console.log();

    console.log(d.attrA);
    console.log(d.attrB);
    // console.log(d.attrC);
    console.log(d.getAttrC());
    console.log();

    let p = new Plane(1000, 999);
    console.log(`speed:${p.speed} maxSpeed:${p.maxSpeed}`);
}

let funcList: Function[] = [
    test0,
    test1,
    test2,
    test3,
    test4,
    test5,
];

function main(): void {
    var c = 5;
    var func = funcList[c];
    func();
}
main()
