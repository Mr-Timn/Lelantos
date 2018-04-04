

Lelantos, the son of Coeus and Phoebe, whose name means "something that
goes unobserved", is the Greek Titan god of air, hunter's skill of stalking
prey, and the unseen. According to greekmythology.com at least.

Lelantos works as a lighter way to hide, prevent access to, and control
folders on your computer. 

Although it has a few different use, its simplest usage is:
	
	To add a new directory
		- adddir -> Paste link -> choose shortcut (Exp. dir1) -> yes -> choose pass
		
	Then hide it. This will only hide it from normal viewing. If show hidden
	folders is on it'll have no effect
		- editdir -> shortcut -> password -> hide
		
	To get around show hidden files. For this to work bp.exe needs to be running.
	Currently its runs as a console application instead of background process for 
	debugging reasons. 
		- editdir -> shortcut -> password -> preventaccess
	
	

TODO
	* Actually implement 'help' command
	* Have bp.exe run on startup
	* Remove directory from list if it no longer exists
	* Add option to drag folder to add it to list of directories
	
	Possibly 
		* Create another process to open bp.exe if it is stopped and vise
		  versa cause virus logic!
		* Create windows application 