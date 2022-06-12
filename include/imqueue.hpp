//
// Created by mdomalew on 11.05.2022.
//
#include <filesystem>
#include "improc.hpp"
#include <map>
#include <iostream>
#ifndef IMAGE_PROCESSOR__SKELETON_IMQUEUE_HPP
#define IMAGE_PROCESSOR__SKELETON_IMQUEUE_HPP


class TaskData{
public:
    std::filesystem::path input_filepath;
    std::filesystem::path output_dir;
    std::function<Image(Image)> processing_function;
    void execute_task(){ //FIXME refactor into a inside function for taskqueue (possibly throw into a cpp file)
        Image pre_exec = load_bitmap(input_filepath.string());
        Image post_exec = processing_function(pre_exec);
        if(!exists(output_dir)){
            create_directory(output_dir);
        }
        save_bitmap(output_dir.string(), post_exec);

    };

};

using queue_type = std::map<std::string, TaskData>;

class TaskQueue{
private:
    queue_type _queue;
public:
    TaskQueue() = default;
    ~TaskQueue() = default;
    void add_task(std::string task_id, TaskData task){
        _queue.insert({task_id , task});
    };
    void execute_all(){
        for(auto task : _queue){
            std::cout << "executing task: " << task.first << std::endl;
            //TODO add execution
        }
    };
    const queue_type& get_all_tasks(){ return _queue; }
};

#endif //IMAGE_PROCESSOR__SKELETON_IMQUEUE_HPP
