const dbms = require("bindings")("dbms");

console.log(dbms.linearSearch)

// create stock
console.log("\ncreate stock")
dbms.linearSearch.addStock("AAPL", "appleId", "100", "1000");
dbms.linearSearch.addStock("GOOGL", "googleId", "200", "2000");
dbms.linearSearch.addStock("AMZN", "amazonId", "300", "3000");
// more examples containing a, but not included above. real company names
dbms.linearSearch.addStock("AB", "abId", "400", "4000");
dbms.linearSearch.addStock("ABC", "abcId", "500", "5000");
dbms.linearSearch.addStock("AAA", "aaaId", "600", "6000");

// get stock
const stock = dbms.linearSearch.getStock("AAPL");
console.log("\nstock", stock)

// update stock
console.log("\nupdate stock")
dbms.linearSearch.updateStock("AAPL", "appleId", "updated field", "2000");

// get stock list
const stockUpdated = dbms.linearSearch.getStock("AAPL");
console.log("\nstockUpdated", stockUpdated)

// delete stock
console.log("\ndelete stock")
dbms.linearSearch.deleteStock("AAPL");

// get stock list
const stockList = dbms.linearSearch.getStockList("A", 1, 10, "name_asc");
console.log("\nstockList", stockList)

// reset db
  // console.log("\nreset db")
  // dbms.linearSearch.resetDatabase();