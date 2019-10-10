
function test0(): void {
    var message:string = "hello typescript";
    console.log(message);
}

function loop(): never {
    while (true) {}
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

let funcList: Function[] = [
    test0,
    test1,
    test2,
];

function main(): void {
    var c = 2;
    var func = funcList[c];
    func();
}
main()
