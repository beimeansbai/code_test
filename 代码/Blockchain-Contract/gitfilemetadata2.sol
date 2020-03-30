pragma solidity >=0.5.14 <0.6.0;
contract gitfilemetadata {

    struct Metadata {
        string owner_id;
        string file_id;//file_git_path+repo_base_hash
        string write_policy;
        string read_policy;
    }
	
	Metadata[] Metadatalist;
	
	event FileMetadata(
		string owner_id,
		string file_id,
		string write_policy,
		string read_policy
	);

    mapping(string => address) metadata_owneraddr;
	address payable contract_owner;


    /// Create this contract
    constructor() public {
		contract_owner = msg.sender;
		Metadatalist.length = 0;
    }
	
	///kill contract itself
	function kill() public {
		if(contract_owner == msg.sender)
			selfdestruct(contract_owner);
		else
			return;
	}
	
	/// Add a new file and its metadata into blockchain
	function add(string memory oid, string memory fid, string memory wp, string memory rp) public {
        for(uint i = 0; i < Metadatalist.length; i++) {
            if (keccak256(abi.encodePacked(fid)) == keccak256(abi.encodePacked(Metadatalist[i].file_id)))
				return;
        }
//		Metadata metadata_cur(oid, fid, wp, rp);
		Metadata memory metadata_cur;
		metadata_cur.owner_id = oid;
		metadata_cur.file_id = fid;
		metadata_cur.write_policy = wp;
		metadata_cur.read_policy = rp;
		metadata_owneraddr[metadata_cur.file_id] = msg.sender;
		Metadatalist.push(metadata_cur);
		emit FileMetadata(metadata_cur.owner_id,metadata_cur.file_id,metadata_cur.write_policy,metadata_cur.read_policy);
    }

    /// Modify metadata of existing file and save into blockchain
    function modify(string memory oid, string memory fid, string memory wp_new, string memory rp_new) public {
		Metadata memory metadata_cur;
		bool isExist = false;
		for(uint i = 0; i < Metadatalist.length; i++) {
            if (keccak256(abi.encodePacked(fid)) == keccak256(abi.encodePacked(Metadatalist[i].file_id))){
				metadata_cur.owner_id = oid;
				metadata_cur.file_id = fid;
				metadata_cur.write_policy = wp_new;
				metadata_cur.read_policy = rp_new;
				isExist = true;
			}
        }
		if (!isExist)
			return;
		else{
			if(msg.sender != metadata_owneraddr[metadata_cur.file_id])  //需不需要判断oid
				return;
			else{
				Metadatalist.push(metadata_cur);
				emit FileMetadata(metadata_cur.owner_id,metadata_cur.file_id,metadata_cur.write_policy,metadata_cur.read_policy);
			}
		}
    }
}
