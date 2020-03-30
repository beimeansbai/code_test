#!/usr/bin/env node
var Web3 = require('web3');
if (typeof web3 !== 'undefined') {
	
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
/*
async function getaccount(){
	var account_one = await web3.eth.getAccounts();
	console.log(account_one[0]);
	//return account_one[0];
}
getaccount();
*/
//var account = getaccount();
//console.log(account);
//console.log(account_one);


//Load the contract:
//abi  ABI JSON
//default_account  address of the account who published the contract
//contractAddr  address of the contract
//https://ropsten.etherscan.io/tx/0x54045da42a06efc9518a1c676a2ac0deef0628e8316960017091bc90e5571c1e
var contractAddr = "0xf43dbef579B7C4ffdCd28Dc2669Dbe99e25AA5D3";      //合约地址
var default_account="0x8F687E0dC0D65a482977a2E344b07F626DbBdb86";     //用户的账户
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


//var oid = "0003";
//var fid = "git/test/b.txt93nwev2";
//var fid = "git/test/a.txt93nfsnv2";
//var wp = "a1 and a3";
//var rp = "a2 or a4";  //可以读文件获取
var fid = process.argv[2];
//console.log(fid);
/*
 var fs = require("fs");
	fs.writeFile("/home/xx/test_file/policy.txt", "policy:", error => {
    if (error) console.log(error);
});
*/

//console.time("the time");
Test.getPastEvents("FileMetadata",{            //获取已存在的事件
	
	fromBlock: 7067287,                     //从较新的块开始搜索
	toBlock: 'latest',
	topics: [null, web3.utils.sha3(fid)]      //根据fid搜索
}, function(error,event){
	if(error)
		console.log(error);
	
	//console.log(event[event.length-1].returnValues.owner_id);
	console.log(event[event.length-1].returnValues.write_policy);      //找到符合条件的metadata，返回读和写策略
	console.log(event[event.length-1].returnValues.read_policy);
	//console.log(event.length);

/*	fs.appendFile("/home/xx/test_file/policy.txt", event[event.length-1].returnValues.write_policy.length, error => {
    if (error) console.log(error);
});
	fs.appendFile("/home/xx/test_file/policy.txt", event[event.length-1].returnValues.write_policy, error => {
    if (error) console.log(error);
});
	fs.appendFile("/home/xx/test_file/policy.txt", event[event.length-1].returnValues.read_policy.length, error => {
    if (error) console.log(error);
});
	fs.appendFile("/home/xx/test_file/policy.txt", event[event.length-1].returnValues.read_policy, error => {
    if (error) console.log(error);
});
*/
	//console.timeEnd("the time");
	process.exitCode = 1;
	process.exit();
});


