1.sorting
	insertion sort:
	8 2 4 9 3 6
	2 8 4 9 3 6
	2 4 8 9 3 6
	2 4 8 9 3 6
	2 3 4 8 9 6
	2 3 4 6 8 9
	for i = 2; i< size; i++
		key = i;
		for j = i-1; j >= 0; j--
			if list[key] < list[j] 
				swap(list[key], list[j])
				key = j;
	
	merge sort:


