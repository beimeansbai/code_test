//init
var Web3 = require('web3');
//console.log(Web3);
if (typeof web3 !== 'undefined') {
	//console.log(Web3);
	web3 = new Web3(web3.currentProvider);
} 
else {
	// set the provider you want from Web3.providers
	//console.log(Web3);
	web3 = new Web3(new Web3.providers.WebsocketProvider('ws://localhost:8546'));
    //web3 = new Web3('http://localhost:8545');

}
//web3.eth.getAccounts(console.log);

//console.log(web3);
//web3.eth.defaultAccount = web3.eth.accounts[0];
//var account_one = web3.eth.defaultAccount;
async function getaccount(){
	var account_one = await web3.eth.getAccounts();
	console.log(account_one[0]);
	//return account_one[0];
}
getaccount();
//var account = getaccount();
//console.log(account);
//console.log(account_one);


//Load the contract:
//abi  ABI JSON
//default_account  address of the account who published the contract
//contractAddr  address of the contract
//https://ropsten.etherscan.io/tx/0x54045da42a06efc9518a1c676a2ac0deef0628e8316960017091bc90e5571c1e
var contractAddr = "0xf43dbef579B7C4ffdCd28Dc2669Dbe99e25AA5D3";
var default_account="0x8F687E0dC0D65a482977a2E344b07F626DbBdb86";
var abi = [
	{
		"inputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "constructor"
	},
	{
		"anonymous": false,
		"inputs": [
			{
				"indexed": false,
				"internalType": "string",
				"name": "owner_id",
				"type": "string"
			},
			{
				"indexed": true,
				"internalType": "string",
				"name": "file_id",
				"type": "string"
			},
			{
				"indexed": false,
				"internalType": "string",
				"name": "write_policy",
				"type": "string"
			},
			{
				"indexed": false,
				"internalType": "string",
				"name": "read_policy",
				"type": "string"
			},
			{
				"indexed": false,
				"internalType": "uint256",
				"name": "update",
				"type": "uint256"
			}
		],
		"name": "FileMetadata",
		"type": "event"
	},
	{
		"constant": false,
		"inputs": [
			{
				"internalType": "string",
				"name": "oid",
				"type": "string"
			},
			{
				"internalType": "string",
				"name": "fid",
				"type": "string"
			},
			{
				"internalType": "string",
				"name": "wp",
				"type": "string"
			},
			{
				"internalType": "string",
				"name": "rp",
				"type": "string"
			}
		],
		"name": "add",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [],
		"name": "kill",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [
			{
				"internalType": "string",
				"name": "oid",
				"type": "string"
			},
			{
				"internalType": "string",
				"name": "fid",
				"type": "string"
			},
			{
				"internalType": "string",
				"name": "wp_new",
				"type": "string"
			},
			{
				"internalType": "string",
				"name": "rp_new",
				"type": "string"
			}
		],
		"name": "modify",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	}
];
var Test = new web3.eth.Contract(abi,contractAddr);
//var TestContract = new web3.eth.Contract(abi);
//var Test = TestContract.at(contractAddr);
//console.log(Test);  //show Test

var oid = "0003";
//var fid = "git/test/b.txt93nwev2";
var fid = "git/test/aaa.txt93nfsnv2";
//var wp = "a1 and a3";
//var rp = "a2 or a4";  //可以读文件获取

//Test.add.sendTransaction(oid, fid, wp, rp, {from:account_one});
//Test.options.from = account_one;
/*  	if(!error)
  		Test.methods.add(oid, fid, wp, rp).send({from: "0x58ba6ade1a167bfc1abba98bcaf685a43dee3ed4"})
  	.on('transactionHash', function(hash){
  	})
  	.on('confirmation', function(confirmationNumber, receipt){
  	})
  	.on('receipt', function(receipt){
  			console.log(receipt);
  	})
  	.on('error', console.error);
});
console.log(faq);*/

//success method
/*
Test.methods.add(oid, fid, wp, rp).send({from:'0x58ba6ade1a167bfc1abba98bcaf685a43dee3ed4'})
.on('transactionHash', function(hash){
	console.log(hash);
 })
.on('confirmation', function(confirmationNumber, receipt){
})
.on('receipt', function(receipt){
	console.log(receipt);
})
.on('error', console.error);
*/


/*
var event = Test.FileMetadata();//可加过滤indexed属性，{base_hash: "aaaa"}

//watch
event.watch(function(error, result){
	console.log("Event are as following:--------");
	
	for(let key in result){
		console.log(key + " : " + result[key]);
	}
	
	console.log("Event ending--------");
});*/
//{filter: {file_id: ["git/test/a.txt93nfsnv2"]}, fromBlock: 0},
 var fs = require("fs");
	fs.writeFile("/home/xx/test_file/policy.txt", 'policy:', error => {
    if (error) console.log(error);
});
console.log(web3.utils.sha3(fid));
Test.events.FileMetadata({
	fromBlock: 7065626,	
	topics: [null, web3.utils.sha3(fid)]
},
function(error,event){
	if(error)
		console.log(error);
	console.log(event);
	console.log('write policy:' + event.returnValues.write_policy);
	console.log('read policy:' + event.returnValues.read_policy);
	//var fs = require("fs");
	fs.appendFile("/home/xx/test_file/policy.txt", event.returnValues.write_policy.length, error => {
    if (error) console.log(error);
});
	fs.appendFile("/home/xx/test_file/policy.txt", event.returnValues.write_policy, error => {
    if (error) console.log(error);
});
	fs.appendFile("/home/xx/test_file/policy.txt", event.returnValues.read_policy.length, error => {
    if (error) console.log(error);
});
	fs.appendFile("/home/xx/test_file/policy.txt", event.returnValues.read_policy, error => {
    if (error) console.log(error);
});

})
.on('data', function(event){
   // console.log(event); 
})
.on('changed', function(event){
    
})
.on('error', function(error, receipt){

});


/*
Test.getPastEvents('FileMetadata',{
	//filter: {file_id: '0x1f7a5b9f20ca2262398ec0d811acdf1b4a24f616b8f45e4a0fa2018709b5a2a7'},
	fromBlock: 0,
	toBlock: 'latest',
	topics: [null, '0x1f7a5b9f20ca2262398ec0d811acdf1b4a24f616b8f45e4a0fa2018709b5a2a7']
}, function(error,event){
	if(error)
		console.log(error);
	console.log(event);
	console.log('read policy:' + event.returnValues);
});
	*/
//.then(function(events){
//	console.log(events)
//});