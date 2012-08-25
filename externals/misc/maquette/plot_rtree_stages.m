function  plot_rtree(testdatafile, nodesfile)
dat = load(testdatafile);

for i=1:size(dat, 1)
    figure;
    hold on;
    
    for j=1:i
        r = [dat(j, 1) dat(j, 3) (dat(j, 2) - dat(j, 1)) (dat(j, 4) - dat(j, 3))];
        rectangle('Position', r);
    end
    
    nodes = load(sprintf('%s_%d.txt', nodesfile, i));
    colors = {'k', 'r', 'g', 'b'};
    stipple = {'--', '--', '--', '--', ':', ':', ':', ':'};
    for j = 1:size(nodes, 1)
        r = [nodes(j, 1) nodes(j, 3) (nodes(j, 2) - nodes(j, 1)) (nodes(j, 4) - nodes(j, 3))];
        rectangle('Position', r, 'LineStyle', stipple{1 + mod(j, length(stipple))}, 'EdgeColor', colors{1 + mod(j, length(colors))});
    end
    % colors = {'k', 'r', 'g', 'b'};
    % stipple = {'--', '--', '--', '--', ':', ':', ':', ':'};
    % for i=1:n2%size(boxes, 1)
    %     r = [boxes(i, 1) boxes(i, 3) (boxes(i, 2) - boxes(i, 1)) (boxes(i, 4) - boxes(i, 3))];
    %     rectangle('Position', r, 'LineStyle', stipple{1 + mod(i, length(stipple))}, 'EdgeColor', colors{1 + mod(i, length(colors))});
    % end
    axis([0,1100, 0, 1100]);    
    hold off;
end

end