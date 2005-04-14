from trac.WikiFormatter import wiki_to_oneliner

def execute(hdf, txt, env):

	if txt == "":
		raise EnvironmentError, 'OOPS!'

	db = env.get_db_cnx()
	
	f=open('/usr/src/gravit/data/commandhelp.txt', 'r')

	line = f.readline()

	while line:
		bits = line.split(' ', 1)
		line = f.readline()
		if bits[0] == txt:
			return wiki_to_oneliner(bits[1], hdf, env, db)
	
	raise EnvironmentError, 'Could not find ' + txt
	
