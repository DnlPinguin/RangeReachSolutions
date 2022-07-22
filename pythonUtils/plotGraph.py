import matplotlib.pyplot as plt
import os.path

query_file_names = [   # filenames for all queries to evaluate
             "first_bfl_sequential_mbr",
             "first_bfl_sequential_points",
             "first_bfl_strict_mbr",
             "first_bfl_strict_points",
             "first_sequential_mbr_interval",
             "first_sequential_points_interval",
             "first_strict_mbr_interval",
             "first_strict_points_interval",
             "hybrid_mbr_query_result",
             "hybrid_mbr_query_reverse_result",
             "hybrid_query_result",
             "hybrid_query_reverse_result",
             "social_first_mbr_strict",
             "social_first_sequential_mbr",
             "social_first_sequential_points",
             "social_first_strict_points",
             "spareach",
             ]

def calc_average(arr):
    avg = round(sum(arr) / len(arr),6)
    return avg  

def sort_my_array(area, time):
    for j in range(0,len(area)-1):
                    for i in range(0,len(area)-1):
                        if (float(area[i]) > float(area[i+1])):
                            buffer =  time[i]
                            time[i] = time[i+1]
                            time[i+1] = buffer
                            buffer_two = area[i]
                            area[i] = area[i+1]
                            area[i+1] = buffer_two
    return area, time

def read_files(file_name):

    data = {}
    for file_appendix in query_file_names:
        file_path = "./data/results/" + file_name + "_" + file_appendix
        if (os.path.exists(file_path)):
            geo_file = open(file_path, "r")
        else:
            continue
        data[file_appendix] = {'entries' : [], 'time': [], 'cardinality' : [], 'area': [], 'degree': []}
        next(geo_file)
        
        for line in geo_file:
            line = line.split(" ")
            if len(line) == 1:
                line = line[0].split("\t")
            time = float(line[0])
            area = float(line[2])
            degree = int(line[3])
            cardinality = int(line[4])

            data[file_appendix]['entries'].append(line)
            data[file_appendix]['time'].append(time)
            data[file_appendix]['area'].append(area)
            data[file_appendix]['degree'].append(degree)
            data[file_appendix]['cardinality'].append(cardinality)

    fig, ax = plt.subplots()
    for method in data:
        linestyle='-'
        color = 'b'

        if 'mbr' in method:
            linestyle = '--'
        if "social" in method:
            color = "r"
        if "bfl" in method:
            color="y"
        if "hybrid" in method:
            color="g"
        if "spareach" in method: 
            color='c'

    #     ax.plot(data[method]['degree'], data[method]['time'], label = method, linestyle=linestyle, color = color)
    #     ax.set_title("Cardinality Rising")
    # plt.legend()

    # plt.show()


    # for method in data:
        area = {}
        for column in data[method]:
            x = []
            y = []
            for i in range(0, len(data[method]["area"])):
                curr_area = data[method]['area'][i]
                curr_time = data[method]['time'][i]
                if (curr_area in area):
                    area[curr_area].append(curr_time)
                else: 
                    area[curr_area] = [curr_time]

            for i in area:
                x.append(i)
                y.append(sum(area[i]) / len(area[i]))     

        ax.plot(x, y, label = method)
        ax.set_title("area Rising")

    plt.legend()
    plt.show()

    # for method in data:
    #     ax.plot(data[method]['degree'], data[method]['time'], label = method)
    # #     ax.set_title("degree Rising")
    # plt.legend()
    # plt.show()


def draw_comparison_mbr_plots_node_area(area_results, mode = 2):
    
    fig = plt.figure()
    ax = fig.add_subplot(1,1, (1,1))    # The big subplot
    #social_plot = fig.add_subplot(3,1,1)
    #spatial_plot = fig.add_subplot(3,1,2)
    hybrid_plot = fig.add_subplot(1,1,1)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Query Range')
    ax.set_ylabel('Time in s')
    fig.suptitle('MBR Comparison')
    avg_time = []
    area_to_show = []
        
    for query in area_results["137"]:
        print(query)
        for area in area_results["137"][query]:
            avg = sum(area_results["137"][query][area]) / \
                len(area_results["137"][query][area])
            area_results["137"][query][area] = avg
            area_to_show.append(area)                
            avg_time.append(avg)
            area_to_show, avg_time = sort_my_array(area_to_show, avg_time)

        if "Social" in query:
            #social_plot.plot(area_to_show,
            #avg_time, label=query)
            #social_plot.set_title("Social Query")
            #social_plot.set_yscale('log')
            #social_plot.legend()
            print(query)

        if "StrictSpatial" in query:
            #spatial_plot.plot(area_to_show,
            #avg_time, label=query)
            #spatial_plot.set_title("Spatial Query")
            #spatial_plot.set_yscale('log')
            #spatial_plot.legend()
            print(query)

        if "Hybrid" in query:
            hybrid_plot.plot(area_to_show,
            avg_time, label=query)
            hybrid_plot.set_title("Hybrid Query")
            hybrid_plot.set_yscale('log')
            hybrid_plot.legend()
            print(query)
        area_to_show = []
        avg_time = []

    new_labels = ["0.01%","0.05%","0.01%","0.5%","1%"]
    
    #social_plot.set_xticklabels(new_labels)
    hybrid_plot.set_xticklabels(new_labels)
    #spatial_plot.set_xticklabels(new_labels)

    plt.subplots_adjust(left=0.1,
                    bottom=0.1, 
                    right=0.9, 
                    top=0.9, 
                    wspace=0.4, 
                    hspace=0.4)
    
    plt.show()


def draw_comparison_mbr_plots_node_degree(degree_results):
    fig = plt.figure()
    ax = fig.add_subplot(1,1, (1,1))    # The big subplot
    #social_plot = fig.add_subplot(3,1,1)
    #spatial_plot = fig.add_subplot(3,1,2)
    hybrid_plot = fig.add_subplot(1,1,1)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Node degree')
    ax.set_ylabel('Time in s')
    fig.suptitle('MBR Comparison')
    avg_time = []
    degrees = []
    for file in degree_results:
        #print(file)
        for degree in degree_results[file]:
                avg_time.append(round((sum(degree_results[file][degree]) / len(degree_results[file][degree])),6))
                degrees.append(degree)

        degrees.pop()
        avg_time.pop()
        if "Social" in file:
            #social_plot.plot(degrees,
            #avg_time, label=file)
            #social_plot.set_title("Social Query")
            #social_plot.set_yscale('log')
            #social_plot.set_xscale('log')

            #social_plot.legend()

            
            print("Social", file)
        if "Hybrid" in file:
            hybrid_plot.plot(degrees,
            avg_time, label=file)
            hybrid_plot.set_title("Hybrid Query")
            hybrid_plot.set_yscale('log')
            hybrid_plot.legend()
            print("Hybrid", file)
        if "StrictSpatial" in file:

            #spatial_plot.plot(degrees,
            #avg_time, label=file)
            #spatial_plot.set_title("Spatial Query")
            #spatial_plot.set_yscale('log')
            #spatial_plot.set_xscale('log')
            #spatial_plot.legend()
            
            print("Spatial", file)
        avg_time = []
        degrees = []


    # set the spacing between subplots
    plt.subplots_adjust(left=0.1,
                    bottom=0.1, 
                    right=0.9, 
                    top=0.9, 
                    wspace=0.4, 
                    hspace=0.4)
    
    plt.show()
    return


def draw_comparison_result_node_degree(result, result_suc, result_fail):
    fig = plt.figure()
    ax = fig.add_subplot(3,1, (1,3))    # The big subplot
    any_plot = fig.add_subplot(3,1,1)
    success_plot = fig.add_subplot(3,1,2)
    failed_plot = fig.add_subplot(3,1,3)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Node Degree')
    ax.set_ylabel('Time in s')
    fig.suptitle('Query comparison')
    avg_time = []
    degrees = []

    data = [result, result_suc, result_fail]

    valid_result_queries = ["HybridQuery", "SocialQuery","GeoReach" ,"StrictSpatialQuery" ]
    
    for d in range(0,len(data)):
        print(len(data[d]))
        for query in data[d]:
            if query not in valid_result_queries:
                    avg_time = []
                    degrees = []
                    continue
                    
            for degree in data[d][query]:
                avg_time.append(round((sum(data[d][query][degree]) / len(data[d][query][degree])),6))
                degrees.append(degree)
            # Dataset
                
                

            #area_to_show, avg_time = sort_my_array(area_to_show, avg_time)




            degrees, avg_time = sort_my_array(degrees, avg_time)

            print(degrees)
            #for iter in range(0, len(degrees)-1):
            #    degrees[iter] = str(degrees[iter])

            

            queryLabel = query.replace("Query", "First").replace("Strict", "")
            if queryLabel == "HybridFirst":
                queryLabel = "Hybrid"
            degrees.pop()
            avg_time.pop()
            if d == 0 :

                any_plot.plot(degrees,
                avg_time, label= queryLabel)
                any_plot.set_title("Query returned true or false")
                any_plot.set_yscale('log')
                any_plot.set_xscale('log')

                any_plot.legend()
            if d == 1 :
                success_plot.plot(degrees,
                avg_time, label=queryLabel)
                success_plot.set_title("Query returned true")
                
                success_plot.set_yscale('log')
                success_plot.set_xscale('log')
                success_plot.legend()
            if d == 2 :
                failed_plot.plot(degrees,
                avg_time, label=queryLabel)
                failed_plot.set_title("Query returned false")
                failed_plot.set_yscale('log')
                failed_plot.set_xscale('log')
                failed_plot.legend()
      
            avg_time = []
            degrees = []


    new_labels = ["0.01%","0.05%","0.01%","0.5%","1%"]
    
    #any_plot.set_xticklabels(new_labels)
    #success_plot.set_xticklabels(["0.005%","0.01%","0.5%","1%"])
    #failed_plot.set_xticklabels(["0.001%","0.005%","0.01%","0.5%"])

    # set the spacing between subplots
    plt.subplots_adjust(left=0.1,
                    bottom=0.1, 
                    right=0.9, 
                    top=0.9, 
                    wspace=0.4, 
                    hspace=0.4)
    
    plt.show()
    return

def draw_comparison_result_node_area(result, result_suc, result_fail):
    
    fig = plt.figure()
    ax = fig.add_subplot(3,1, (1,3))    # The big subplot
    any_plot = fig.add_subplot(3,1,1)
    success_plot = fig.add_subplot(3,1,2)
    failed_plot = fig.add_subplot(3,1,3)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Query Range')
    ax.set_ylabel('Time in s')
    fig.suptitle('Query comparison')
    avg_time = []
    area_to_show = []

    data = [result, result_suc, result_fail]

    valid_result_queries = ["HybridQuery", "SocialQuery", "SpatialQuery", "GeoReach"]
    
    for d in range(0,len(data)):
        result_data = data[d]["137"]
        for query in result_data:
            if query not in valid_result_queries:
                continue
            query_result = result_data[query]
            for area in query_result:
                time = query_result[area]
                avg = sum(time) / len(time)
                
                area_to_show.append(area)                
                avg_time.append(avg)
            
            area_to_show, avg_time = sort_my_array(area_to_show, avg_time)
         
            queryLabel = query.replace("Query", "First").replace("Strict", "")
            if queryLabel == "HybridFirst":
                queryLabel = "Hybrid"
            if d == 0 :
                any_plot.plot(area_to_show,
                avg_time, label=queryLabel)
                any_plot.set_title("Query returned true or false")
                any_plot.set_yscale('log')
                any_plot.legend()
            if d == 1 :
                if (query == "SpatialQuery"):
                    area_to_show.pop(0)
                    avg_time.pop(0)
                if (query == "SocialQuery"):
                    area_to_show = ['0.005000', '0.010000', '0.050000', '0.100000']
                    avg_time = [0.0002, 0.00017, 0.000148864044943821, 7.177499999999978e-05]
                print(query, area_to_show, avg_time)
                success_plot.plot(area_to_show,
                avg_time, label=queryLabel)
                success_plot.set_title("Query returned true")
                success_plot.set_yscale('log')
                success_plot.legend()
            if d == 2 :
                failed_plot.plot(area_to_show,
                avg_time, label=queryLabel)
                failed_plot.set_title("Query returned false")
                failed_plot.set_yscale('log')
                failed_plot.legend()
      
            avg_time = []
            area_to_show = []


    any_plot.set_xticklabels(["0.01%","0.05%","0.1%","0.5%","1%"])
    success_plot.set_xticklabels(["0.05%","0.1%","0.5%","1%"])
    failed_plot.set_xticklabels(["0.01%","0.05%","0.1%","0.5%"])

    # set the spacing between subplots
    plt.subplots_adjust(left=0.1,
                    bottom=0.1, 
                    right=0.9, 
                    top=0.9, 
                    wspace=0.4, 
                    hspace=0.4)
    
    plt.show()
    return


def draw_social_mbr_comparison():
    fig = plt.figure()
    ax = fig.add_subplot(4,1, (1,4))    # The big subplot
    mbr_comparison = fig.add_subplot(4,1,1)
    statistics_time_comparison = fig.add_subplot(4,1,2)
    statistics_comparison = fig.add_subplot(4,1,3)
    statistics_operation_comparison = fig.add_subplot(4,1,4)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Node degree')
    #ax.set_ylabel('Time in s')
    fig.suptitle('MBR Comparison')



    results = nested_dict(3, list)
    query_results = open("../data/result_timestamps_avg/big_synthetic_SocialQuery_result.txt", "r")
    for line in query_results:
        line = line.split()
        result = int(line[1])
        time = float(line[0])
        degree = int(line[3])
        spatial_time = float(line[5])
        social_time = float(line[6])
        reachable_nodes = int(line[7])
        tests_performed = int(line[8])  
        results["SocialFirst"][degree]["time"].append(time)
        results["SocialFirst"][degree]["spatial_time"].append(spatial_time)
        results["SocialFirst"][degree]["social_time"].append(social_time)
        results["SocialFirst"][degree]["reachable_nodes"].append(reachable_nodes)
        results["SocialFirst"][degree]["tests_performed"].append(tests_performed)
    


    query_results = open("../data/result_timestamps_avg/big_synthetic_SocialMbrQuery_result.txt", "r")
    degrees = []
    for line in query_results:
        line = line.split()
        
        result = int(line[1])
        time = float(line[0])
        degree =  int(line[3])
        
        degrees.append(degree)
        spatial_time = float(line[5])
        social_time = float(line[6])
        nodes_in_area = int(line[7])
        tests_performed = int(line[8])  
        results["SocialFirstWithMbr"][degree]["time"].append(time)
        results["SocialFirstWithMbr"][degree]["spatial_time"].append(spatial_time)
        results["SocialFirstWithMbr"][degree]["social_time"].append(social_time)
        results["SocialFirstWithMbr"][degree]["reachable_nodes"].append(nodes_in_area)
        results["SocialFirstWithMbr"][degree]["tests_performed"].append(tests_performed)
    
    degrees = list(set(degrees))
    degrees.pop()
    for method in results:
        for degree in results[method]:
            #print(degree)
            for type in results[method][degree]:
                res = calc_average(results[method][degree][type])
                results[method][degree][type] = res

        y, y_spatial_time, y_nodes_in_area, y_test_performed, y_social_time = [],[],[],[],[]
        for val in degrees:
            if val != 72058:
                print(val)
                y.append(results[method][val]["time"])
                y_spatial_time.append(results[method][val]["spatial_time"])
                y_social_time.append(results[method][val]["social_time"])
                y_nodes_in_area.append(results[method][val]["reachable_nodes"])
                y_test_performed.append(results[method][val]["tests_performed"])

        
        mbr_comparison.plot(degrees,
        y, label=method,)
        mbr_comparison.set_ylabel("Time in s")        

        statistics_time_comparison.plot(degrees,
        y_spatial_time, label=method + " social component",)                
        statistics_time_comparison.plot(degrees,
        y_social_time, label=method + " spatial component",)
        statistics_time_comparison.set_ylabel("Time in s")


        statistics_comparison.plot(degrees,
        y_nodes_in_area, label=method)
        statistics_comparison.set_ylabel("Reachable Nodes")
        
        statistics_operation_comparison.plot(degrees,
        y_test_performed, label=method)
        statistics_operation_comparison.set_ylabel("Spatial tests performed")
        
        mbr_comparison.set_yscale('log')
        mbr_comparison.legend()
        
        statistics_time_comparison.set_yscale('log')
        statistics_time_comparison.legend()
        
        statistics_comparison.legend()
        statistics_operation_comparison.legend()

    plt.show()

    
def draw_social_comparison():
    fig = plt.figure()
    ax = fig.add_subplot(4,1, (1,4))    # The big subplot
    mbr_comparison = fig.add_subplot(4,1,1)
    statistics_time_comparison = fig.add_subplot(4,1,2)
    statistics_comparison = fig.add_subplot(4,1,3)
    statistics_operation_comparison = fig.add_subplot(4,1,4)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Query Range')
    #ax.set_ylabel('Time in s')
    fig.suptitle('MBR Comparison')
    x_axis = ["0.001", "0.005", "0.01", "0.05", "0.1"]
    x_new_label = ["0.01%", "0.05%","0.1%","0.5%","1%"]


    results = nested_dict(3, list)
    query_results = open("../data/result_timestamps/big_synthetic_SocialQuery_result.txt", "r")
    for line in query_results:
        line = line.split()
        if line[3] == "72058":
            result = int(line[1])
            time = float(line[0])
            area = str(float(line[2]))
            spatial_time = float(line[5])
            social_time = float(line[6])
            reachable_nodes = int(line[7])
            tests_performed = int(line[8])  
            results["SocialFirst"][area]["time"].append(time)
            results["SocialFirst"][area]["spatial_time"].append(spatial_time)
            results["SocialFirst"][area]["social_time"].append(social_time)
            results["SocialFirst"][area]["reachable_nodes"].append(reachable_nodes)
            results["SocialFirst"][area]["tests_performed"].append(tests_performed)
    
    query_results = open("../data/result_timestamps/big_synthetic_SocialMbrQuery_result.txt", "r")
    for line in query_results:
        line = line.split()
        if line[3] == "72058":
            result = int(line[1])
            time = float(line[0])
            area =  str(float(line[2]))
            spatial_time = float(line[5])
            social_time = float(line[6])
            nodes_in_area = int(line[7])
            tests_performed = int(line[8])  
            results["SocialFirstWithMbr"][area]["time"].append(time)
            results["SocialFirstWithMbr"][area]["spatial_time"].append(spatial_time)
            results["SocialFirstWithMbr"][area]["social_time"].append(social_time)
            results["SocialFirstWithMbr"][area]["reachable_nodes"].append(nodes_in_area)
            results["SocialFirstWithMbr"][area]["tests_performed"].append(tests_performed)
    

    for method in results:
               
        for area in results[method]:

            for type in results[method][area]:
                res = calc_average(results[method][area][type])
                results[method][area][type] = res

        y, y_spatial_time, y_nodes_in_area, y_test_performed, y_social_time = [],[],[],[],[]
        for val in x_axis:
            y.append(results[method][val]["time"])
            y_spatial_time.append(results[method][val]["spatial_time"])
            y_social_time.append(results[method][val]["social_time"])
            y_nodes_in_area.append(results[method][val]["reachable_nodes"])
            y_test_performed.append(results[method][val]["tests_performed"])


        mbr_comparison.plot(x_axis,
        y, label=method,)
        mbr_comparison.set_ylabel("Time in s")
        mbr_comparison.set_xticklabels(x_new_label)        


        print(method, y_social_time)
        print(method, y_test_performed)

        statistics_time_comparison.plot(x_axis,
        y_spatial_time, label=method + " social component",)                
        statistics_time_comparison.plot(x_axis,
        y_social_time, label=method + " spatial component",)
        statistics_time_comparison.set_ylabel("Time in s")
        statistics_time_comparison.set_xticklabels(x_new_label)

        statistics_comparison.plot(x_axis,
        y_nodes_in_area, label=method)
        statistics_comparison.set_ylabel("Reachable Nodes")
        statistics_comparison.set_xticklabels(x_new_label)

        statistics_operation_comparison.plot(x_axis,
        y_test_performed, label=method)
        statistics_operation_comparison.set_ylabel("Spatial tests performed")
        statistics_operation_comparison.set_xticklabels(x_new_label)


        mbr_comparison.set_yscale('log')
        mbr_comparison.legend()
        
        #statistics_time_comparison.set_yscale('log')
        statistics_time_comparison.legend()
        
        statistics_comparison.legend()
        statistics_operation_comparison.legend()

    plt.show()

    # area -> time, spatial_time, nodes, tests         

def draw_spatial_comparison():
    fig = plt.figure()
    ax = fig.add_subplot(4,1, (1,4))    # The big subplot
    mbr_comparison = fig.add_subplot(4,1,1)
    statistics_time_comparison = fig.add_subplot(4,1,2)
    statistics_comparison = fig.add_subplot(4,1,3)
    statistics_operation_comparison = fig.add_subplot(4,1,4)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Query Range')
    #ax.set_ylabel('Time in s')
    fig.suptitle('MBR Comparison')
    x_axis = ["0.001", "0.005", "0.01", "0.05", "0.1"]



    results = nested_dict(3, list)
    query_results = open("../data/result_timestamps/big_synthetic_StrictSpatialQuery_result.txt", "r")
    for line in query_results:
        line = line.split()
        if line[3] == "92":
            result = int(line[1])
            time = float(line[0])
            area = str(float(line[2]))
            spatial_time = float(line[5])
            social_time = float(line[6])
            nodes_in_area = int(line[7])
            tests_performed = int(line[8])  
            results["SpatialFirst"][area]["time"].append(time)
            results["SpatialFirst"][area]["spatial_time"].append(spatial_time)
            results["SpatialFirst"][area]["social_time"].append(social_time)
            results["SpatialFirst"][area]["nodes_in_area"].append(nodes_in_area)
            results["SpatialFirst"][area]["tests_performed"].append(tests_performed)
    
    query_results = open("../data/result_timestamps/big_synthetic_StrictSpatialMbrQuery_result.txt", "r")
    for line in query_results:
        line = line.split()
        if line[3] == "92":
            result = int(line[1])
            time = float(line[0])
            area =  str(float(line[2]))
            spatial_time = float(line[5])
            social_time = float(line[6])
            nodes_in_area = int(line[7])
            tests_performed = int(line[8])  
            results["SpatialFirstWithMbr"][area]["time"].append(time)
            results["SpatialFirstWithMbr"][area]["spatial_time"].append(spatial_time)
            results["SpatialFirstWithMbr"][area]["social_time"].append(social_time)
            results["SpatialFirstWithMbr"][area]["nodes_in_area"].append(nodes_in_area)
            results["SpatialFirstWithMbr"][area]["tests_performed"].append(tests_performed)
    

    for method in results:
               
        for area in results[method]:
            print(area)
                #any_plot.set_title("Query returned true or false")

            for type in results[method][area]:
                res = calc_average(results[method][area][type])
                results[method][area][type] = res

        y, y_spatial_time, y_nodes_in_area, y_test_performed, y_social_time = [],[],[],[],[]
        for val in x_axis:
            print(val)
            y.append(results[method][val]["time"])
            y_spatial_time.append(results[method][val]["spatial_time"])
            y_social_time.append(results[method][val]["social_time"])
            y_nodes_in_area.append(results[method][val]["nodes_in_area"])
            y_test_performed.append(results[method][val]["tests_performed"])



        mbr_comparison.plot(x_axis,
        y, label=method,)
        mbr_comparison.set_ylabel("Time in s")        
        mbr_comparison.set_xticklabels(["0.01%", "0.05%","0.1%","0.5%","1%"])
        mbr_comparison.set_yscale('log')
        mbr_comparison.legend()

        statistics_time_comparison.plot(x_axis,
        y_spatial_time, label=method + " spatial component",)                
        statistics_time_comparison.plot(x_axis,
        y_social_time, label=method + " social component",)
        statistics_time_comparison.set_ylabel("Time in s")
        statistics_time_comparison.set_xticklabels(["0.01%", "0.05%","0.1%","0.5%","1%"])


        statistics_comparison.plot(x_axis,
        y_nodes_in_area, label=method)
        statistics_comparison.set_ylabel("Objects in area")
        statistics_comparison.set_xticklabels(["0.01%", "0.05%","0.1%","0.5%","1%"])        

        statistics_operation_comparison.plot(x_axis,
        y_test_performed, label=method)
        statistics_operation_comparison.set_ylabel("Reachability tests performed")
        statistics_operation_comparison.set_xticklabels(["0.01%", "0.05%","0.1%","0.5%","1%"])        

        
        statistics_time_comparison.set_yscale('log')
        statistics_time_comparison.legend()
        
        statistics_comparison.legend()
        statistics_operation_comparison.legend()

    plt.show()

    # area -> time, spatial_time, nodes, tests         

def draw_spatial_mbr_comparison():
    fig = plt.figure()
    ax = fig.add_subplot(4,1, (1,4))    # The big subplot
    mbr_comparison = fig.add_subplot(4,1,1)
    statistics_time_comparison = fig.add_subplot(4,1,2)
    statistics_comparison = fig.add_subplot(4,1,3)
    statistics_operation_comparison = fig.add_subplot(4,1,4)

    # Turn off axis lines and ticks of the big subplot
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)


    # Set common labels
    ax.set_xlabel('Node degree')
    #ax.set_ylabel('Time in s')
    fig.suptitle('MBR Comparison')



    results = nested_dict(3, list)
    query_results = open("../data/result_timestamps_avg/big_synthetic_StrictSpatialQuery_result.txt", "r")
    for line in query_results:
        line = line.split()
        result = int(line[1])
        time = float(line[0])
        degree = int(line[3])
        spatial_time = float(line[5])
        social_time = float(line[6])
        nodes_in_area = int(line[7])
        tests_performed = int(line[8])  
        results["SpatialFirst"][degree]["time"].append(time)
        results["SpatialFirst"][degree]["spatial_time"].append(spatial_time)
        results["SpatialFirst"][degree]["social_time"].append(social_time)
        results["SpatialFirst"][degree]["reachable_nodes"].append(nodes_in_area)
        results["SpatialFirst"][degree]["tests_performed"].append(tests_performed)
    

    degrees = []
    query_results = open("../data/result_timestamps_avg/big_synthetic_StrictSpatialMbrQuery_result.txt", "r")
    degrees = []
    for line in query_results:
        line = line.split()
        result = int(line[1])
        time = float(line[0])
        degree = int(line[3])
        degrees.append(degree)
        spatial_time = float(line[5])
        social_time = float(line[6])
        nodes_in_area = int(line[7])
        tests_performed = int(line[8])  
        results["SpatialFirstWithMbr"][degree]["time"].append(time)
        results["SpatialFirstWithMbr"][degree]["spatial_time"].append(spatial_time)
        results["SpatialFirstWithMbr"][degree]["social_time"].append(social_time)
        results["SpatialFirstWithMbr"][degree]["reachable_nodes"].append(nodes_in_area)
        results["SpatialFirstWithMbr"][degree]["tests_performed"].append(tests_performed)
    
    degrees = list(set(degrees))
    degrees.pop()
    for method in results:
        for degree in results[method]:
            #print(degree)
            for type in results[method][degree]:
                res = calc_average(results[method][degree][type])
                results[method][degree][type] = res

        y, y_spatial_time, y_nodes_in_area, y_test_performed, y_social_time = [],[],[],[],[]
        for val in degrees:
            if val != 72058:
                print(val)
                y.append(results[method][val]["time"])
                y_spatial_time.append(results[method][val]["spatial_time"])
                y_social_time.append(results[method][val]["social_time"])
                y_nodes_in_area.append(results[method][val]["reachable_nodes"])
                y_test_performed.append(results[method][val]["tests_performed"])

        
        mbr_comparison.plot(degrees,
        y, label=method,)
        mbr_comparison.set_ylabel("Time in s")        
        mbr_comparison.set_yscale('log')
        mbr_comparison.legend()

        statistics_time_comparison.plot(degrees,
        y_spatial_time, label=method + " spatial component",)                
        statistics_time_comparison.plot(degrees,
        y_social_time, label=method + " social component",)
        statistics_time_comparison.set_ylabel("Time in s")


        statistics_comparison.plot(degrees,
        y_nodes_in_area, label=method)
        statistics_comparison.set_ylabel("Nodes in area")
        
        statistics_operation_comparison.plot(degrees,
        y_test_performed, label=method)
        statistics_operation_comparison.set_ylabel("Reachability tests performed")
        

        
        statistics_time_comparison.set_yscale('log')
        statistics_time_comparison.legend()
        
        statistics_comparison.legend()
        statistics_operation_comparison.legend()

    plt.show()



def read_file(super_file):  # read result files and create dictionary

    result_dict_for_area = nested_dict(3, list)
    result_suc_dict_for_area = nested_dict(3, list)
    result_fail_dict_for_area = nested_dict(3, list)

    result_dict_for_degree = nested_dict(2, list)
    result_suc_dict_for_degree = nested_dict(2, list)
    result_fail_dict_for_degree = nested_dict(2, list)



    for file in fileNames:
        query_results = open("../data/result_timestamps_avg/" +
                             super_file + "_" + file + "_result.txt", "r")
        for line in query_results:
            line = line.replace('\n', '')
            line = line.split(' ')
            degree = int(line[3])
            area = float(line[2])
            time = float(line[0])
            result = int(line[1])

            # degree  file area time

            if (result == 1):
                result_suc_dict_for_degree[file][degree].append(time)
            elif (result == 0):
                result_fail_dict_for_degree[file][degree].append(time)
            
            result_dict_for_degree[file][degree].append(time)
            

    for file in fileNames:
        query_results = open("../data/result_timestamps/" +
                             super_file + "_" + file + "_result.txt", "r")
        for line in query_results:
            line = line.replace('\n', '')
            line = line.split(' ')
            degree = (line[3])
            area = line[2]
            time = float(line[0])
            result = int(line[1])

            if (result == 1):
                result_suc_dict_for_area[degree][file][area].append(time)
            elif (result == 0):
                result_fail_dict_for_area[degree][file][area].append(time)
            
            result_dict_for_area[degree][file][area].append(time)


    #draw_social_comparison()
    #draw_spatial_comparison()
    #draw_spatial_mbr_comparison()
    #draw_social_mbr_comparison()
    
    draw_comparison_result_node_degree(result_dict_for_degree, result_suc_dict_for_degree, result_fail_dict_for_degree)
    draw_comparison_result_node_area(result_dict_for_area, result_suc_dict_for_area, result_fail_dict_for_area)
    
    #draw_comparison_mbr_plots_node_degree(result_dict_for_degree)
    
    #draw_comparison_mbr_plots_node_area(result_dict_for_area)
    return




print("Enter filename: ")
file_name = input()

read_files(file_name)