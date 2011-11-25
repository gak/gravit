
for i in *.dll; do bas=`basename $i .dll`; echo $bas; pexports $i | sed "s/^_//" > $bas.def; done

for i in *.dll; do bas=`basename $i .dll`; echo $bas; dlltool -U -d $bas.def -l $bas.a; done 
