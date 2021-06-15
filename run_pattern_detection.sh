#! /bin/sh
echo "################################################################################"
echo " 		Patterns Detection"
echo ""
echo "For more information: daniellekwadjo@gmail.com"
echo "################################################################################"

./../gspan/example/gspan --input DFG/patterns/input.txt --output DFG/patterns/output.txt --minsupp 0.008 -e autgrp

exit 0
