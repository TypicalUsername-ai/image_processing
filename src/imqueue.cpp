//
// Created by mdomalew on 11.05.2022.
//
#include "imqueue.hpp"
#include "improc.hpp"

void TaskQueue::execute_all() {
    for(auto task : _queue){
        std::cout << "executing task: " << task.first << std::endl;
        task.second.execute_task(task.first);
    }
}

void TaskData::execute_task(std::string _id) {
    Image pre_exec = load_bitmap(input_filepath.string());
    Image post_exec = processing_function(pre_exec);
    if(!exists(output_dir)){
        bool a = create_directory(output_dir);
        std::cout << a << std::endl;
    }
    std::string path_file = output_dir.append(input_filepath.filename().string()).string();
    path_file = path_file.substr(0, path_file.length() - 4) + "__" + _id + input_filepath.extension().string();
    std::cout << "saving to: " << path_file << std::endl;
    int exit = save_bitmap( path_file, post_exec);
    if(exit == EXIT_FAILURE){
        std::cout << "saving failed!" << std::endl;
    }
    std::cout << "saved in:\n" << std::filesystem::absolute(output_dir).string() << std::endl;

};