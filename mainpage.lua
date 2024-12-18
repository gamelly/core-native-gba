function main()
    local file = io.open(arg[1], 'r')

    if not file then
        return
    end

    local content = file:read('*a')

    content = content:gsub('---\n\n:whale: .*\n', '')
    content = content:gsub('| :warning: Attention', '')
    content = content:gsub('| :------------------ ', '')
    content = content:gsub('| there is no sanitization,', '@warning there is no sanitization,')
    content = content:gsub('|\n', '\n')
    content = content:gsub('\n## Building: .* `Gly Engine`\n', '\n## Gly Engine\n')
    content = content:gsub('\n## Building: .* `Love2D`\n', '\n## Love2D\n')
    content = content:gsub('\n## Building: .* Engine\n', '\n## Custom Engine\n')

    io.write(content)
end

main()
