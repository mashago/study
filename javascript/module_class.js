
function World() {
    var name;
    this.setName = function (n) {
        this.name = n;
    }
    this.printName = function () {
        console.log(this.name);
    }
}
module.exports = World;
