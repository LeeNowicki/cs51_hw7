

	
	.global fun


//Calculates the factorial of input 1
//I tested this by testing each number in sequence, it looks right
//up until 20!, after which it overflows, with 21! overflowing at 3. 

fun:
	//push %rdi  // save the input
	//mov $string1, %rdi
 	//call puts
	//pop %rax   // get the input back
	//add $2, %rax 
  
  //Set up helper function for factorial
  mov $1, %rax
  
  //Save arg to factorial with and decrement
  push %rdi

  //Test if zero first (special case, returns 1)
  and %rdi, %rdi
  je end
  
  //Okay reload and save it again
  pop %rdi
  push %rdi
  
  call fact

//Just to have somewhere to jump if zero input
end:
  pop %rdi
	ret

//calculates factorial of original %rdi, decrementing along the way
fact:
  mul %rdi  

  //If this caused a carry, jump to overflowed
  jc overflowed
  
  //Now decrement and jump if non-zero
  dec %rdi

  jne fact

  //If it gets here, then it got all the way to zero
  //Return
  ret
  
  
  
  
//Print "Overflowed!" and load in whatever it failed on into %rax  
overflowed:
  //Now print the overflow message
  push %rdi
  mov $string1, %rdi
  call puts
  pop %rdi

  mov %rdi, %rax


  ret
  

  
  

string1:
	.asciz "Overflowed!"
