classdef test_CBIG_spin_test_rotation_surf < matlab.unittest.TestCase
% Written by Ruby Kong and CBIG under MIT license: http://github.com/ThomasYeoLab/CBIG/blob/master/LICENSE.md
    
    methods (Test)
        
        % test input matrix case
        function matrixCase(testCase)
            % get replace_unittest_flag
            CBIG_CODE_DIR = getenv('CBIG_CODE_DIR');
            load(fullfile(CBIG_CODE_DIR, 'unit_tests','replace_unittest_flag'));
            
            % get the current output 
            [lh_rot_fs5, rh_rot_fs5] = CBIG_spin_test_rotation_surf('fsaverage5', 1, 1);
            [lh_rot_fslr, rh_rot_fslr] = CBIG_spin_test_rotation_surf('fs_LR_32k', 1, 1);
            
            if replace_unittest_flag
                % replace reference result
                save('ref_output/rotation_results.mat','lh_rot_fs5','rh_rot_fs5','lh_rot_fslr','rh_rot_fslr')
            else
                ref_results = load('ref_output/rotation_results.mat');

                % compare the current output with expected output
                assert(sum((lh_rot_fs5 - ref_results.lh_rot_fs5) ~= 0) < 10,...
                    sprintf('lh fsaverage5 rotation result off by %f vertices',...
                    sum((lh_rot_fs5 - ref_results.lh_rot_fs5) ~= 0)));
                assert(sum((rh_rot_fs5 - ref_results.rh_rot_fs5) ~= 0) < 10,...
                    sprintf('rh fsaverage5 rotation result off by %f vertices',...
                    sum((rh_rot_fs5 - ref_results.rh_rot_fs5) ~= 0)));
                assert(sum((lh_rot_fslr - ref_results.lh_rot_fslr) ~= 0) < 10,...
                    sprintf('lh fs_LR_32k rotation result off by %f vertices',...
                    sum((lh_rot_fslr - ref_results.lh_rot_fslr) ~= 0)));
                assert(sum((rh_rot_fslr - ref_results.rh_rot_fslr) ~= 0) < 10,...
                    sprintf('rh fs_LR_32k rotation result off by %f vertices',...
                    sum((rh_rot_fslr - ref_results.rh_rot_fslr) ~= 0)));
            end
        end

    end
    
end
