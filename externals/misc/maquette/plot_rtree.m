function  plot_rtree(testdata, nodes)
dat = load(testdata);
nodes = load(nodes);

    hold on;
    
    for j=1:size(dat, 1)
        r = [dat(j, 1) dat(j, 3) (dat(j, 2) - dat(j, 1)) (dat(j, 4) - dat(j, 3))];
        rectangle('Position', r);
    end
    
    colors = {'k', 'r', 'g', 'b'};
    stipple = {'--', '--', '--', '--', ':', ':', ':', ':'};
    for i=1:size(nodes, 1)
        r = [nodes(i, 1) nodes(i, 3) (nodes(i, 2) - nodes(i, 1)) (nodes(i, 4) - nodes(i, 3))];
        rectangle('Position', r, 'LineStyle', stipple{1 + mod(i, length(stipple))}, 'EdgeColor', colors{1 + mod(i, length(colors))});
    end
axis([0,1100, 0, 1100]);    
    hold off;
end