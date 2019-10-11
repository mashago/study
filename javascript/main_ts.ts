
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

let funcList: Function[] = [
    test0,
    test1,
    test2,
    test3,
];

function main(): void {
    var c = 3;
    var func = funcList[c];
    func();
}
main()
