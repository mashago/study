
print("******* show dbs")
db.getMongo().getDBs()

print("******* use test")
db.getSisterDB("test")

print("******* show collections")
db.getCollectionNames()

var data = 
{
	"null_var" : null
,	"bool_var" : true
,	"float_var" : 3.14
,	"int_var" : NumberInt("10")
,	"string_var" : "masha"
,	"date_var" : new Date()
,	"array_var" : ["a", "b", 111]
,	"doc_var" : {"foo" : "bar"}
,	"obj_var" : ObjectId()
,	"func_var" : function() { print("this is func_var") }
}

print("******* insert")
db.test_data.insert(data)

print("******* findOne")
db.test_data.findOne()

print("******* remove")
db.test_data.remove({})
