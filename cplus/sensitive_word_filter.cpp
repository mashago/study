
#include <stdio.h>
#include "sensitive_word_filter.h"

/////// sensitive_node_t START [

sensitive_node_t::sensitive_node_t()
{
	deep = 0;
	end = false;
}

void sensitive_node_t::insert(std::string word)
{
	// use substr, slow
	// 1.if word empty, return
	// 2.find by first char form word in node_map, if node not exists, new one
	// 3.if word only has 1 char, set its child node is end, return
	// 4.child node insert last [1,end-1] string
	
	if (word.size() == 0)
	{
		// root node ignore empty word
		return;
	}

	sensitive_node_t *child_node = NULL;
	auto iter = node_map.find(word[0]);
	if (iter == node_map.end())
	{
		child_node = new sensitive_node_t();
		child_node->deep = deep + 1;
		node_map.insert(std::make_pair(word[0], child_node));
	}
	else
	{
		child_node = iter->second;
	}
		
	if (word.size() == 1)
	{
		// set child node is end
		child_node->end = true;
		return;
	}

	child_node->insert(word.substr(1, word.size()-1));
}

void sensitive_node_t::insert2(const char *word, size_t size)
{
	// use pointer, faster
	// 1.if word empty, return
	// 2.find by first char form word in node_map, if node not exists, new one
	// 3.if word only has 1 char, set its child node is end, return
	// 4.child node insert last [1,end-1] string
	
	if (size == 0)
	{
		// root node ignore empty word
		return;
	}

	sensitive_node_t *child_node = NULL;
	auto iter = node_map.find(*word);
	if (iter == node_map.end())
	{
		child_node = new sensitive_node_t();
		child_node->deep = deep + 1;
		node_map.insert(std::make_pair(*word, child_node));
	}
	else
	{
		child_node = iter->second;
	}
		
	if (size == 1)
	{
		// set child node is end
		child_node->end = true;
		return;
	}

	child_node->insert2(word+1, size-1);
}

void sensitive_node_t::insert3(const std::string &word, size_t pos, size_t size)
{
	// use reference
	// 1.if word empty, return
	// 2.find by first char form word in node_map, if node not exists, new one
	// 3.if word only has 1 char, set its child node is end, return
	// 4.child node insert last [1,end-1] string
	
	if (size == 0)
	{
		// root node ignore empty word
		return;
	}

	sensitive_node_t *child_node = NULL;
	auto iter = node_map.find(word[pos]);
	if (iter == node_map.end())
	{
		child_node = new sensitive_node_t();
		child_node->deep = deep + 1;
		node_map.insert(std::make_pair(word[pos], child_node));
	}
	else
	{
		child_node = iter->second;
	}
		
	if (size == 1)
	{
		// set child node is end
		child_node->end = true;
		return;
	}

	child_node->insert3(word, pos+1, size-1);
}



bool sensitive_node_t::search(std::string word)
{
	// use substr, slow
	// 1.if node is end, return true
	// 2.if word is empty, node is not end, return false
	// 3.find by first char form word in node_map, if node not exists, return false
	// 4.child node serach last [1,end-1] string 
	
	if (end == true)
	{
		return true;
	}

	if (word.size() == 0)
	{
		return false;
	}

	auto iter = node_map.find(word[0]);
	if (iter == node_map.end())
	{
		return false;
	}

	sensitive_node_t *child_node = iter->second;
	return child_node->search(word.substr(1, word.size()-1));
}

bool sensitive_node_t::search2(const char *word, size_t size)
{
	// use pointer, faster
	// 1.if node is end, return true
	// 2.if word is empty, node is not end, return false
	// 3.find by first char form word in node_map, if node not exists, return false
	// 4.child node serach last [1,end-1] string 
	
	if (end == true)
	{
		return true;
	}

	if (size == 0)
	{
		return false;
	}

	auto iter = node_map.find(*word);
	if (iter == node_map.end())
	{
		return false;
	}

	sensitive_node_t *child_node = iter->second;
	return child_node->search2(word+1, size-1);
}

bool sensitive_node_t::search3(const std::string &word, size_t pos, size_t size)
{
	// use reference
	// 1.if node is end, return true
	// 2.if word is empty, node is not end, return false
	// 3.find by first char form word in node_map, if node not exists, return false
	// 4.child node serach last [1,end-1] string 
	
	if (end == true)
	{
		return true;
	}

	if (size == 0)
	{
		return false;
	}

	auto iter = node_map.find(word[pos]);
	if (iter == node_map.end())
	{
		return false;
	}

	sensitive_node_t *child_node = iter->second;
	return child_node->search3(word, pos+1, size-1);
	return false;
}


const sensitive_node_t * sensitive_node_t::find_node(const char *word, size_t size)
{
	// get the match node
	// 1.if node is end, try find a deeper child node, if not exists , return this, else return deeper node
	// 2.find in node_map
	
	if (end == true)
	{
		// find if has more deeper node
		if (size > 0)
		{
			auto iter = node_map.find(*word);
			if (iter == node_map.end())
			{
				return this;
			}

			sensitive_node_t *child_node = iter->second;
			const sensitive_node_t *node = child_node->find_node(word+1, size-1);
			if (node != NULL)
			{
				return node;
			}
		}
		return this;
	}

	if (size == 0)
	{
		return NULL;
	}

	auto iter = node_map.find(*word);
	if (iter == node_map.end())
	{
		return NULL;
	}

	sensitive_node_t *child_node = iter->second;
	return child_node->find_node(word+1, size-1);
}

void sensitive_node_t::clear()
{
	// 1.child node do clear()
	// 2.delete child node
	
	for (auto iter = node_map.begin(); iter != node_map.end(); iter++)
	{
		iter->second->clear();
		delete iter->second;
	}
}

/////// sensitive_node_t END ]

/////// sensitive_filter_t START [

sensitive_filter_t::sensitive_filter_t()
{
	root = new sensitive_node_t();
}

sensitive_filter_t::~sensitive_filter_t()
{
	root->clear();
}

void sensitive_filter_t::load(std::set<std::string> words)
{
	for (auto iter = words.begin(); iter != words.end(); iter++)
	{
		root->insert(*iter);
	}
}

void sensitive_filter_t::load2(std::set<std::string> words)
{
	for (auto iter = words.begin(); iter != words.end(); iter++)
	{
		root->insert2((*iter).c_str(), (*iter).size());
	}
}

void sensitive_filter_t::load3(std::set<std::string> words)
{
	for (auto iter = words.begin(); iter != words.end(); iter++)
	{
		root->insert3(*iter, 0, (*iter).size());
	}
}


bool sensitive_filter_t::find(std::string input)
{
	if (input.size() == 0)
	{
		return false;
	}

	for (size_t i = 0; i < input.size(); i++)
	{
		std::string t = input.substr(i, input.size()-i);
		if (root->search(t) == true)
		{
			return true;
		}
	}
	return false;
}

bool sensitive_filter_t::find2(const std::string &input)
{
	// use pointer, faster
	if (input.size() == 0)
	{
		return false;
	}

	for (size_t i = 0; i < input.size(); i++)
	{
		if (root->search2(input.c_str()+i, input.size()-i) == true)
		{
			return true;
		}
	}
	return false;
}

bool sensitive_filter_t::find3(const std::string &input)
{
	// use reference
	if (input.size() == 0)
	{
		return false;
	}

	for (size_t i = 0; i < input.size(); i++)
	{
		if (root->search3(input, i, input.size()-i) == true)
		{
			return true;
		}
	}
	return false;
}


bool sensitive_filter_t::replace(std::string &input)
{
	// 1.find match node, if no exists, pos++
	// 2.if exists, set [pos, pos+node.deep] as '*', pos += node.deep
	
	if (input.size() == 0)
	{
		return false;
	}

	bool has_replace = false;

	for (size_t i = 0; i < input.size(); )
	{
		const sensitive_node_t *node = root->find_node(input.c_str()+i, input.size()-i);
		if (node != NULL)
		{
			has_replace = true;
			for (size_t j = i; j < i + node->deep; j++)
			{
				input[j] = '*';
			}
			i += node->deep;
			continue;
		}
		i++;
	}

	return has_replace;
}

/////// sensitive_filter_t END ]

