
#!/bin/sh

DIR=./diagramscripts
LANG=C make -p | python3 ${DIR}/make_p_to_json.py | python3 ${DIR}/json_to_dot.py | dot -Tpdf >| ${DIR}/workflow.pdf