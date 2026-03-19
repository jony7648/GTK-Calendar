#include <iostream>
#include "sort.h"

int out_small_vecs(std::vector<std::vector<std::string>>& line_vec, int min_len, int left_bord, int right_bord) {
	size_t partition_size = right_bord - left_bord + 1;

	int p=left_bord, i=left_bord, j=right_bord-1;


	//find a pivot
	while (p < j && line_vec.at(p).size() < min_len) {
		p++;
	}


	i = p;

	//quick sort stuff
	while (j>i) {
		std::vector<std::string>& p_vec = line_vec.at(p);
		std::vector<std::string>& i_vec = line_vec.at(i);
		std::vector<std::string>& next_i_vec = line_vec.at(i+1);

		
		if (next_i_vec.size() < min_len) {
			std::swap(p_vec, next_i_vec);
			p++;
			i++;
		}

		else if (next_i_vec.size() < min_len) {
			std::swap(next_i_vec, p_vec);
			i++;
			p++;
		}
		else if (next_i_vec.size() >= min_len)  {
			std::vector<std::string>& j_vec = line_vec.at(j);

			if (j_vec.size() < min_len) {
				std::swap(next_i_vec, j_vec);
			}

			j--;
		}
	}

	return i;
}

namespace util {
void sort_2d_vec(std::vector<std::vector<std::string>>& line_vec, int left_bord, int right_bord, int str_check_index, int char_check_index) {
	//this function will sort a a vector with multiple subvectors alphabeticly
	if (left_bord >= right_bord - 1) {
		//std::cout << "ALL DONE\n";
		return;
	}

	int mid_left_bord = 0, right_mid_bord = 0;
	int vec_i_index = left_bord, vec_j_index = right_bord - 1, piv_index = left_bord; //used as the regular i, j and piv, pointers in quick sort

	//the actual i, j and p pointers
	std::vector<std::string> *p_i_vec = &line_vec.at(0), *p_next_i_vec = nullptr, *p_j_vec = nullptr, *p_piv_vec = &line_vec.at(piv_index);

	//used to refrence strings in the subvectors
	std::string *p_piv_str = nullptr, *p_i_str = nullptr, *p_next_i_str = nullptr, *p_j_str = nullptr;
	char piv_char, i_char, next_i_char, j_char;


	int partition_size = vec_j_index - vec_i_index;
	int pre_swaps = 0;

	
	pre_swaps = line_vec.back().size() <= str_check_index;


	//remove vectors that are too that have been the exact same up to this point
	//but are now too small to continue being checked
	left_bord = out_small_vecs(line_vec, str_check_index+1, left_bord, right_bord);
	partition_size = right_bord - left_bord;
	vec_i_index = left_bord;
	piv_index = left_bord;


	//if all vectors were outted exit this set of recursions
	if (left_bord == vec_j_index) {
		return;
	}


	//move vectors that are currently being checked to the back if they have less charcaters
	//than the minmum char check count
	for (int j=left_bord+partition_size-1; j>=left_bord; j--) {
		p_piv_vec = &line_vec.at(piv_index);
		p_j_vec = &line_vec.at(j);
		const std::string& j_str = p_j_vec->at(str_check_index);	



		if (char_check_index >= j_str.length()) {
			std::swap(*p_j_vec, *p_piv_vec);
			piv_index++;
			vec_i_index++;
			pre_swaps++;
		}
	}

	//if all the characters needed to be swapped then the strings being checked are the same
	//move to the next one
	if (pre_swaps >= partition_size) {
		sort_2d_vec(line_vec, left_bord, right_bord, str_check_index+1, 0);
		return;
	}



	//prevent the loop form iterating too many times
	int max_iter = 300;

	

	//do regular quick sort stuff
	while (vec_i_index != vec_j_index) {
		max_iter--;

		if (max_iter < 0) {
			std::cout << "Too many iterations\n";
			break;
		}

		p_piv_vec = &line_vec.at(piv_index);
		p_i_vec = &line_vec.at(vec_i_index);
		p_j_vec = &line_vec.at(vec_j_index);
		p_next_i_vec = &line_vec.at(vec_i_index + 1);

		p_piv_str = &p_piv_vec->at(str_check_index);
		p_i_str = &p_i_vec->at(str_check_index);
		p_j_str = &p_j_vec->at(str_check_index);
		p_next_i_str = &p_next_i_vec->at(str_check_index);

		piv_char = p_piv_str->at(char_check_index);
		i_char = p_i_str->at(char_check_index);
		j_char = p_j_str->at(char_check_index);
		next_i_char = p_next_i_str->at(char_check_index);
		
		if (next_i_char == piv_char) {
			vec_i_index++;
			continue;
		}

		if (i_char > next_i_char) {
			std::swap(*p_piv_vec, *p_next_i_vec);
			vec_i_index++;
			piv_index++;
			continue;
		}

		if (i_char < next_i_char) {
			if (j_char <= piv_char) {
				std::swap(*p_next_i_vec, *p_j_vec);
			}
			
			vec_j_index--;
			continue;
		}
	}

	//for charcaters that were the same move to the next character and repeat the process
	sort_2d_vec(line_vec, piv_index, vec_i_index+1, str_check_index, char_check_index+1);

	sort_2d_vec(line_vec, left_bord, piv_index, str_check_index, char_check_index);
	sort_2d_vec(line_vec, vec_i_index+1, right_bord, str_check_index, char_check_index);
}
}
