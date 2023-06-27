void swap(int* a, int* b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void reverse(int* a, int* b)
{
	while (a < b)
	{
		swap(a++, b--);
	}
}

bool permutation(int* start, int* end)
{
	int* left = end;
	int* right, *rightest_larger;
	while (left != start)
	{
		right = left;
		left--;

		if (*left < *right)
		{
			rightest_larger = end;
			while (*rightest_larger <= *left)
			{
				rightest_larger--;
			}

			swap(left, rightest_larger);
			reverse(right, end);

			return true;
		}
	}

	reverse(start, end);
	return false;
}