#include <iostream>
#include <list>
#include <map>
#include <vector>

struct request_entry {
    int size;
    int id;
};

map<int, int> free_blocks_id;
multimap<int, int> free_blocks_size;

void add_block(int id, int size) {
    free_blocks_id.insert( { id, size } );
    free_blocks_size.insert( { size, id } );
}

void remove(map<int, int>::iterator& a) {
    auto b = free_blocks_size.find(a -> second);
    while (b -> second != a -> first) b++;
    free_blocks_size.erase(b);
    free_blocks_id.erase(a);
}

int main() {
    int max_size, req_count, id, size;
    cin >> max_size >> req_count;
    request_entry requests[req_count];
    add_block(1, max_size);

    for (int curr_req_id = 0; curr_req_id < req_count; curr_req_id++) {
        int input; cin >> input;

        if (input > 0) {
            auto selected_block = free_blocks_size.lower_bound(input);
            if (selected_block == free_blocks_size.end()) id = -1;
            else {
                id = selected_block->second;
                int new_id = id + input;
                size = selected_block->first - input;

                free_blocks_id.erase(selected_block->second);
                free_blocks_size.erase(selected_block);

                if (size != 0) add_block(new_id, size);
            }
            cout << id << endl;

        }
        else {
            int id_x = requests[abs(input) - 1].id;
            int size_x = requests[abs(input) - 1].size;
            if (id_x == -1) continue;

            auto right = free_blocks_id.lower_bound(id_x);
            auto left = (right != free_blocks_id.begin()) ? prev(right) : free_blocks_id.end();

            if (right != free_blocks_id.end() && right->first == id_x + size_x){
                if (left != free_blocks_id.end() && left->first + left->second == id_x) {
                    id = left->first;
                    size = left->second + right->second;
                    remove(left);
                    remove(right);
                    add_block(id, size + size_x);
                } else {
                    size = right->second;
                    remove(right);
                    add_block(id_x, size + size_x);
                }
            } else {
                if (left != free_blocks_id.end() && left->first + left->second == id_x) {
                    id = left->first;
                    size = left->second;
                    remove(left);
                    add_block(id, size + size_x);
                } else add_block(id_x, size_x);
            }
        }
        requests[curr_req_id] = {input, id};
    }
}