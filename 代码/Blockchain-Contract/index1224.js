//init
var Web3 = require("web3");

if (typeof web3 !== 'undefined') {
  web3 = new Web3(web3.currentProvider);
} 
else {
  // set the provider you want from Web3.providers
  web3 = new Web3(new Web3.providers.HttpProvider("https://ropsten.infura.io/ + key"));

}

web3.eth.defaultAccount = web3.eth.accounts[0];
var account_one = web3.eth.accounts[0];

//Load the contract:
//abi  ABI JSON
//default_account  address of the account who published the contract
//contractAddr  address of the contract
//https://ropsten.etherscan.io/tx/0x54045da42a06efc9518a1c676a2ac0deef0628e8316960017091bc90e5571c1e
var contractAddr = "0x58279e1d9259f6e88d2551af5f9195c7e578a28d";
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
				"indexed": false,
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
console.log(Test);  //show Test

var oid = "";
var fid = "";
var wp = "";
var rp = "";  //可以读文件获取
//Test.add.sendTransaction(oid, fid, wp, rp, {from:account_one});
//Test.options.from = account_one;

Test.methods.add(oid, fid, wp, rp).send({from: '0x58ba6ade1a167bfc1abba98bcaf685a43dee3ed4'})
.on('transactionHash', function(hash){
})
.on('confirmation', function(confirmationNumber, receipt){
})
.on('receipt', function(receipt){
	console.log(receipt);
})
.on('error', console.error);

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

Test.events.FileMetadata(function(error,event){console.log(event);})
.on('data', function(event){
    console.log(event); 
})
.on('changed', function(event){
    
})
.on('error', console.error);
