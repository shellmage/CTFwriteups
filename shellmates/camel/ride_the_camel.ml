let print_banner () =
	print_endline "";
	print_endline "                                                        =--_";
	print_endline "                                         .-\"\"\"\"\"\"-.     |* _)";
	print_endline "                                        /          \\   /  /";
	print_endline "                                       /            \\_/  /";
	print_endline "           _                          /|                /";
	print_endline "       _-'\"/\\                        / |    ____    _.-\"            _";
	print_endline "    _-'   (  '-_            _       (   \\  |\\  /\\  ||           .-'\".\".";
	print_endline "_.-'       '.   `'-._   .-'\"/'.      \"   | |/ /  | |/        _-\"   (   '-_";
	print_endline "             '.      _-\"   (   '-_       \\ | /   \\ |     _.-'       )     \"-._";
	print_endline "           _.'   _.-'       )     \"-._    ||\\\\   |\\\\  '\"'        .-'";
	print_endline "         '               .-'          `'  || \\\\  ||))";
	print_endline "jjs__  _  ___  _ ____________ _____  ___ _|\\ _|\\_|\\\\/ _______________  ___   _";
	print_endline "                       c  c  \" c C \"\"C  \" \"\"  \"\" \"\"";
	print_endline "                   c       C";
	print_endline "              C        C";
	print_endline "                   C";
	print_endline "    C     c";;
let string_length s =
	let len = ref 0 in
	String.iter (fun c -> len := !len + 1) s;
	!len;;

let list_of str =
	let rec aux newlst = function
	| -1 -> newlst;
	| k -> aux (str.[k]::newlst) (k - 1) in
	aux [] ((string_length str) - 1);;

let rec check = function
	| 369877807 -> true;
	| k when k > 369877807 -> false;
	| k -> check (2*k+1);;
	
let rec simp = function	
	| k when k mod 2 = 1 -> k;
	| k -> simp (k / 2);;

let decrypt data _key =
	let key = simp (_key + 1) in
	print_int key;
	print_newline();
	List.iter (fun k -> 
		print_char (Char.chr (    ((((k lxor (key land 0xff)) + ((key lsr 8) land 0xff)) lxor ((key lsr 16) land 0xff)) - ((key lsr 24) land 0xff)) land 0xff));
	) data;
	print_newline();;

let take_a_ride () =
	print_banner();
	print_endline "Who are you?";
	let s1 = read_line () in
	if not (String.equal s1 "Xavier") then (
		print_endline "You are not allowed to use this program"; 
		exit 1;
	);
	print_endline "Welcome, enter the secret number";
	let s2 = read_int () in
	if (s2 <> 369877807) && (check s2) then
		decrypt [85;97;98;121;102;119;100;107;124;119;100;127;97;98;101;183;183;183;176;68;120;123;176;122;124;119;121;176;127;101;176;174;176;101;120;123;124;124;99;119;100;123;101;109;105;120;160;113;125;98;160;105;101;113;119;118;160;107;100;113;160;85;119;99;124;81;83] s2
			else print_endline "Wrong secret number";;
take_a_ride ();
